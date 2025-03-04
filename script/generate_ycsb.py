#!/usr/bin/python3

import sys
import os
from pathlib import Path

script_dir = Path(os.path.realpath(__file__))
ycsb_dir = "YCSB/"
workload_dir = str(script_dir.parent.parent) + "/workload_specification/"
output_dir = "workloads/"

if not os.path.exists(output_dir):
    os.makedirs(output_dir)

if not os.path.exists(ycsb_dir):
    os.system("curl -O --location https://github.com/brianfrankcooper/YCSB/releases/download/0.16.0/ycsb-0.16.0.tar.gz")
    os.system("tar xfvz ycsb-0.16.0.tar.gz")
    os.system("mv ycsb-0.16.0 " + ycsb_dir)
    os.system("rm ycsb-0.16.0.tar.gz")

workload = "workloada"
key_type = "randint"

if len(sys.argv) == 1:
    print("Usage: python", sys.argv[0], "<workload_type> <key_type>")
    print("Example: 'python", sys.argv[0], "a randint' for generating workloada with randint.")

if len(sys.argv) > 1:
    workload = str(sys.argv[1])
    if not workload.startswith("workload"):
        workload = "workload" + workload

if len(sys.argv) > 2:
    key_type = sys.argv[2]

print("Generaring YCSB workload", workload, "with key type", key_type)

out_ycsb_load = output_dir + 'ycsb_load_' + key_type + '_' + workload
out_ycsb_txn = output_dir + 'ycsb_txn_' + key_type + '_' + workload
out_load_ycsbkey = output_dir + 'load_' + 'ycsbkey' + '_' + workload
out_txn_ycsbkey = output_dir + 'txn_' + 'ycsbkey' + '_' + workload
out_load = output_dir + 'fill_' + key_type + '_' + workload
out_txn = output_dir + 'mixed_' + key_type + '_' + workload

cmd_ycsb_load = ycsb_dir + 'bin/ycsb.sh load basic -P ' + workload_dir + workload + ' -s > ' + out_ycsb_load
cmd_ycsb_txn = ycsb_dir + 'bin/ycsb.sh run basic -P ' + workload_dir + workload + ' -s > ' + out_ycsb_txn

os.system(cmd_ycsb_load)
os.system(cmd_ycsb_txn)

#####################################################################################

with open(out_ycsb_load, 'r') as f_load:
    with open(out_load_ycsbkey, 'w') as f_load_out:
        for line in f_load :
            cols = line.split()
            if len(cols) > 0 and cols[0] == "INSERT":
                f_load_out.write (cols[0] + " " + cols[2][4:] + "\n")

with open (out_ycsb_txn, 'r') as f_txn:
    with open (out_txn_ycsbkey, 'w') as f_txn_out:
        for line in f_txn :
            cols = line.split()
            if (cols[0] == 'SCAN') or (cols[0] == 'INSERT') or (cols[0] == 'READ') or (cols[0] == 'UPDATE'):
                startkey = cols[2][4:]
                if cols[0] == 'SCAN' :
                    numkeys = cols[3]
                    f_txn_out.write (cols[0] + ' ' + startkey + ' ' + numkeys + '\n')
                else :
                    f_txn_out.write (cols[0] + ' ' + startkey + '\n')
cmd = 'rm -f ' + out_ycsb_load
os.system(cmd)
cmd = 'rm -f ' + out_ycsb_txn
os.system(cmd)

if key_type == 'randint' :
    with open (out_load_ycsbkey, 'r') as f_load:
        with open (out_load, 'w') as f_load_out:
            for line in f_load :
                f_load_out.write (line)

    with open (out_txn_ycsbkey, 'r') as f_txn:
        with open (out_txn, 'w') as f_txn_out:
            for line in f_txn :
                f_txn_out.write (line)

elif key_type == 'monoint' :
    keymap = {}
    with open (out_load_ycsbkey, 'r') as f_load:
        with open (out_load, 'w') as f_load_out:
            count = 0
            for line in f_load :
                cols = line.split()
                keymap[int(cols[1])] = count
                f_load_out.write (cols[0] + ' ' + str(count) + '\n')
                count += 1

    with open (out_txn_ycsbkey, 'r') as f_txn:
        with open (out_txn, 'w') as f_txn_out:
            for line in f_txn :
                cols = line.split()
                if cols[0] == 'SCAN' :
                    f_txn_out.write (cols[0] + ' ' + str(keymap[int(cols[1])]) + ' ' + cols[2] + '\n')
                elif cols[0] == 'INSERT' :
                    keymap[int(cols[1])] = count
                    f_txn_out.write (cols[0] + ' ' + str(count) + '\n')
                    count += 1
                else :
                    f_txn_out.write (cols[0] + ' ' + str(keymap[int(cols[1])]) + '\n')


cmd = 'rm -f ' + out_load_ycsbkey
os.system(cmd)
cmd = 'rm -f ' + out_txn_ycsbkey
os.system(cmd)
