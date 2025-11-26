#!/usr/bin/env python3
# experiment_runner.py
# Ejecuta main para todos los archivos en selected_instances para fb y bt,
# con variantes: todos / micro / macro y distintos K. Mide tiempos y guarda CSV.

import os
import subprocess
import time
import csv
import sys
import shlex
from pathlib import Path

ROOT = Path(".").resolve()
INST_DIR = ROOT / "selected_instances"
BINARY = ROOT / "tp1_bin"

# === Configurables ===
COMPILE_CMD = "g++ -std=c++17 -O2 -pipe *.cpp -o tp1_bin"
TIMEOUT_SECONDS = 120
ALGORITHMS = ["fb", "bt"]
VARIANTS = ["todos", "micro", "macro"]   # "todos" no requiere K
K_VALUES = [5, 10]                        # ajustá los K que quieras correr

def read_instance_header(filepath):
    with open(filepath, "r") as f:
        first = f.readline().strip()
        if first == "":
            return None
        try:
            N = int(first.split()[0])
        except:
            N = int(first)
        second = f.readline().strip()
        M = int(second.split()[0])
        return N, M

def parse_solver_stdout(stdout_text):
    lines = stdout_text.splitlines()
    info = {
        "influencers": [],
        "cost": None,
        "stdout_snippet": "\n".join(lines[-40:])  # últimos 40 para debug
    }
    for line in lines:
        line = line.strip()
        if line.startswith("Influencers usados:"):
            parts = line.split(":", 1)[1].strip()
            if parts == "":
                info["influencers"] = []
            else:
                try:
                    info["influencers"] = [int(x) for x in parts.split()]
                except:
                    info["influencers"] = []
        if line.startswith("Costo total:"):
            try:
                info["cost"] = int(line.split(":",1)[1].strip())
            except:
                info["cost"] = None
    return info

def compute_covered_count_from_solution(instance_path, influencer_indices):
    # OJO: esto solo es correcto para "todos".
    # En micro/macro los índices cambian por el filtrado en C++.
    with open(instance_path, "r") as f:
        lines = f.read().splitlines()
    if len(lines) < 2:
        return 0, 0
    N = int(lines[0].strip())
    M = int(lines[1].strip())
    influencer_segments = []
    for i in range(M):
        idx = 2 + i
        if idx >= len(lines):
            influencer_segments.append([])
            continue
        line = lines[idx].strip()
        if line == "":
            influencer_segments.append([])
            continue
        tokens = [t.strip() for t in line.split(',')]
        segs = []
        for tok in tokens[1:]:
            if tok == "": continue
            try:
                segs.append(int(tok))
            except:
                pass
        influencer_segments.append(segs)
    covered = set()
    for inf in influencer_indices:
        if 0 <= inf < len(influencer_segments):
            for s in influencer_segments[inf]:
                covered.add(s)
    return len(covered), N

def compile_all():
    print("Compilando todos los .cpp con:\n", COMPILE_CMD)
    r = subprocess.run(COMPILE_CMD, shell=True)
    if r.returncode != 0:
        print("Error al compilar. Revisa errores.")
        sys.exit(1)
    print("Compilación OK. Binario:", BINARY)

def run_experiments(output_csv="experiment_results_k.csv"):
    if not INST_DIR.exists():
        print("No encontré la carpeta selected_instances en:", INST_DIR)
        sys.exit(1)

    instances = sorted([p.name for p in INST_DIR.iterdir() if p.is_file()])
    if not instances:
        print("No hay instancias en selected_instances/")
        sys.exit(1)

    with open(output_csv, "w", newline='') as csvfile:
        writer = csv.writer(csvfile)
        # Agregamos variant y K al CSV
        header = ["instance","algorithm","variant","K","N","M","time_seconds",
                  "cost","num_influencers","covered_count","success","stdout_snippet"]
        writer.writerow(header)

        for inst in instances:
            inst_path = INST_DIR / inst
            nm = read_instance_header(inst_path)
            if nm is None:
                print("No pude leer N,M de", inst)
                continue
            N, M = nm

            for algo in ALGORITHMS:
                for variant in VARIANTS:
                    # construir lista de comandos a ejecutar (1 o varios K)
                    if variant == "todos":
                        cmds = [([f"./{BINARY.name}", inst, algo], None)]
                    else:
                        cmds = [([f"./{BINARY.name}", inst, algo, variant, str(K)], K)
                                for K in K_VALUES]

                    for cmd_args, Kval in cmds:
                        printable = " ".join(cmd_args)
                        print("Ejecutando:", printable)
                        start = time.perf_counter()
                        try:
                            proc = subprocess.run(cmd_args,
                                                  cwd=str(ROOT),
                                                  stdout=subprocess.PIPE,
                                                  stderr=subprocess.STDOUT,
                                                  timeout=TIMEOUT_SECONDS,
                                                  text=True)
                            elapsed = time.perf_counter() - start
                            out = proc.stdout
                            parsed = parse_solver_stdout(out)
                            influencers = parsed["influencers"]
                            cost = parsed["cost"]
                            num_infl = len(influencers)

                            # covered_count/success: solo confiable en "todos".
                            if variant == "todos":
                                covered_count, totalN = compute_covered_count_from_solution(inst_path, influencers)
                                success = 1 if covered_count == totalN else 0
                            else:
                                covered_count = ""
                                success = ""  # no verificable fuera del binario

                            writer.writerow([
                                inst, algo, variant, (Kval if Kval is not None else ""),
                                N, M, f"{elapsed:.6f}",
                                cost if cost is not None else "",
                                num_infl, covered_count, success, parsed["stdout_snippet"]
                            ])
                            print(f" -> {inst} {algo} {variant}"
                                  f"{'' if Kval is None else f' K={Kval}'}: "
                                  f"time={elapsed:.3f}s cost={cost} influencers={num_infl} "
                                  f"{'(covered OK)' if success==1 else ''}")

                        except subprocess.TimeoutExpired:
                            elapsed = time.perf_counter() - start
                            writer.writerow([inst, algo, variant, (Kval if Kval is not None else ""),
                                             N, M, f"{elapsed:.6f}", "TIMEOUT", "", "", 0, "TIMEOUT"])
                            print(f"TIMEOUT: {inst} {algo} {variant}"
                                  f"{'' if Kval is None else f' K={Kval}'} after {elapsed:.2f}s")
                        except Exception as e:
                            elapsed = time.perf_counter() - start
                            writer.writerow([inst, algo, variant, (Kval if Kval is not None else ""),
                                             N, M, f"{elapsed:.6f}", "ERROR", "", "", 0, f"ERROR: {e}"])
                            print(f"ERROR running {printable}: {e}")

    print("Experimento finalizado. Resultados en", output_csv)

if __name__ == "__main__":
    compile_all()
    run_experiments()
