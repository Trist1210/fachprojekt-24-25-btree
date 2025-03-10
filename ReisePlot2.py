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
filename = "Reise.md"
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
    insert_times.append(insert_sum / 5)
    lookup_times.append(lookup_sum / 5)
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
plt.figure(figsize=(10, 10))
axs = plt.axes()
bar_width = 0.35
index = range(len(titles))
index_diff = range(1, len(titles))

# Erster Plot: Absolute Zeiten
axs.invert_yaxis()
axs.bar(index_diff, prozent_insert, bar_width, label="Insert Time Difference")
axs.bar([i + bar_width for i in index_diff], prozent_lookup, bar_width, label="Lookup Time Difference")
axs.set_xlabel("")
axs.set_ylabel("Time Difference (%)")
axs.set_title("Prozentuale Laufzeitverbesserung pro Optimierung" )
axs.set_xticks([i + bar_width / 2 for i in index_diff])
axs.set_xticklabels(titles[1:], rotation=0, ha="center")
axs.legend()


# Werte über die Balken schreiben
for i, v in enumerate(prozent_insert):
    axs.text(i+1, v, f"{v:.2f}%", ha='center', va='bottom')
for i, v in enumerate(prozent_lookup):
    axs.text(i+1 + bar_width, v, f"{v:.2f}%", ha='center', va='bottom')

plt.tight_layout()
plt.savefig("reise2_output.png", format='png')
plt.show()
