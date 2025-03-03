import matplotlib.pyplot as plt
import numpy as np
import re

# Datei einlesen
def parse_file(filename):
    with open(filename, "r", encoding="utf-8") as f:
        data = f.readlines()
    
    results = {}
    current_title = None

    for line in data:
        title_match = re.match(r"!(.*)", line)
        if title_match:
            current_title = title_match.group(1).strip()
            results[current_title] = {"insert": [], "lookup": []}
        
        insert_match = re.match(r"Insert phase\[(\d)\]: seconds: ([0-9.]+)", line)
        lookup_match = re.match(r"Lookup phase\[(\d)\]: seconds: ([0-9.]+)", line)
        
        if insert_match and current_title:
            phase_index = int(insert_match.group(1))  # Die Phase (0-9)
            insert_time = float(insert_match.group(2))  # Die Zeit
            results[current_title]["insert"].append((phase_index, insert_time))  # Phase und Zeit speichern
        
        # Überprüfen, ob ein Lookup-Match vorliegt und die entsprechende Phase speichern
        elif lookup_match and current_title:
            phase_index = int(lookup_match.group(1))  # Die Phase (0-9)
            lookup_time = float(lookup_match.group(2))  # Die Zeit
            results[current_title]["lookup"].append((phase_index, lookup_time))  # Phase und Zeit speichern
    
    return results

# Daten verarbeiten
filename = "Test Results.md"
data = parse_file(filename)
insert_times = []
lookup_times = []
titles = list(data.keys())
for title in titles:
    insert_sum = 0
    lookup_sum = 0
    for phase, time in data[title]["insert"]:
        insert_sum += time
    for phase, time in data[title]["lookup"]:
        lookup_sum += time
    insert_times.append(insert_sum / 10)
    lookup_times.append(lookup_sum / 10)
#insert_times = [data[title]["insert"] for title in titles]
#lookup_times = [data[title]["lookup"] for title in titles]

# Differenzen berechnen (nur negative Werte)
diff_insert = [insert_times[i] - insert_times[i-1] for i in range(1, len(insert_times))]
diff_lookup = [lookup_times[i] - lookup_times[i-1] for i in range(1, len(lookup_times))]

prozent_insert = []
prozent_lookup = []
for i in range(1, len(insert_times)):
    prozent_insert.append(((insert_times[i] - insert_times[i-1]) / insert_times[i-1])*100)
for i in range(1, len(lookup_times)):
    prozent_lookup.append(((lookup_times[i] - lookup_times[i-1]) / lookup_times[i-1])*100)

# Veränderung vom ersten zum letzten Wert für die Kreisdiagramme
insert_change = abs(insert_times[-1] - insert_times[0])
lookup_change = abs(lookup_times[-1] - lookup_times[0])
# Plot erstellen
fig, axs = plt.subplots(4, 1, figsize=(10, 20))
bar_width = 0.35
index = range(len(titles))
index_diff = range(1, len(titles))

# Erster Plot: Absolute Zeiten
axs[0].bar(index, insert_times, bar_width, label="Insert Time")
axs[0].bar([i + bar_width for i in index], lookup_times, bar_width, label="Lookup Time")
axs[0].set_xlabel("")
axs[0].set_ylabel("Time (seconds)")
axs[0].set_title("Insert vs Lookup Time per Title")
axs[0].set_xticks([i + bar_width / 2 for i in index])
axs[0].set_xticklabels(titles, rotation=0, ha="center")
axs[0].legend()

# Zweiter Plot: Nur negative Differenzen
axs[1].invert_yaxis()
axs[1].bar(index_diff, diff_insert, bar_width, label="Insert Time Difference")
axs[1].bar([i + bar_width for i in index_diff], diff_lookup, bar_width, label="Lookup Time Difference")
axs[1].set_xlabel("")
axs[1].set_ylabel("Time Difference (seconds)")
axs[1].set_title("Insert vs Lookup Time Difference per Title")
axs[1].set_xticks([i + bar_width / 2 for i in index_diff])
axs[1].set_xticklabels(titles[1:], rotation=0, ha="center")
axs[1].legend()

# Werte über die Balken schreiben
for i, v in enumerate(insert_times):
    axs[0].text(i, v, f"{v:.3f}s", ha='center', va='bottom')
for i, v in enumerate(lookup_times):
    axs[0].text(i + bar_width, v, f"{v:.3f}s", ha='center', va='bottom')

# Werte über die Balken schreiben
for i, v in enumerate(diff_insert):
    axs[1].text(i+1, v, f"{v:.3f}s", ha='center', va='bottom')
for i, v in enumerate(diff_lookup):
    axs[1].text(i+1 + bar_width, v, f"{v:.3f}s", ha='center', va='bottom')

# Dritter Plot: Kreisdiagramm für Insert Change
#x1=diff_insert[0]*-1
#x2=diff_insert[1]*-1
#x3=diff_insert[2]
#axs[3].pie([x1,x2,x3], labels=["Insert Change", "First Insert", "Last Insert"], autopct='%1.1f%%', startangle=90, colors=['blue', 'gray', 'lightblue'])
#axs[3].set_title("Insert Change from First to Last Title")
y1=diff_lookup[0]*-1
y2=diff_lookup[1]*-1
y3=diff_lookup[2]*-1
# Vierter Plot: Kreisdiagramm für Lookup Change

axs[2].pie([y1,y2,y3], labels=["Multi-threading", "batching", "simd"], autopct='%1.1f%%', startangle=90, colors=['orange', 'gray', 'lightcoral'],pctdistance=1.05)
axs[2].set_title("Lookup Change from First to Last Title")


# Vierter Plot: Gestapeltes Balkendiagramm für Lookup Change
axs[3].invert_yaxis()
axs[3].bar(index_diff, prozent_insert, bar_width, label="Insert Time Difference")
axs[3].bar([i + bar_width for i in index_diff], prozent_lookup, bar_width, label="Lookup Time Difference")
axs[3].set_xlabel("")
axs[3].set_ylabel("Time Difference (%)")
axs[3].set_title("Insert vs Lookup Time Difference per Title")
axs[3].set_xticks([i + bar_width / 2 for i in index_diff])
axs[3].set_xticklabels(titles[1:], rotation=0, ha="center")
axs[3].legend()

# Werte über die Balken schreiben
for i, v in enumerate(prozent_insert):
    axs[3].text(i+1, v, f"{v:.1f}%", ha='center', va='bottom')
for i, v in enumerate(prozent_lookup):
    axs[3].text(i+1 + bar_width, v, f"{v:.1f}%", ha='center', va='bottom')

plt.tight_layout()
plt.savefig("chart_output.pdf", format='pdf')
plt.show()
