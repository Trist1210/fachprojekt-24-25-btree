import matplotlib.pyplot as plt
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
            results[current_title] = {"insert": None, "lookup": None}
        
        insert_match = re.match(r"Insert phase\[0\]: seconds: ([0-9.]+)", line)
        lookup_match = re.match(r"Lookup phase\[0\]: seconds: ([0-9.]+)", line)
        
        if insert_match and current_title:
            results[current_title]["insert"] = float(insert_match.group(1))
        elif lookup_match and current_title:
            results[current_title]["lookup"] = float(lookup_match.group(1))
    
    return results

# Daten verarbeiten
filename = "Test Results.md"
data = parse_file(filename)

titles = list(data.keys())
insert_times = [data[title]["insert"] for title in titles]
lookup_times = [data[title]["lookup"] for title in titles]

# Differenzen berechnen
diff_insert = [insert_times[i] - insert_times[i-1] for i in range(1, len(insert_times))]
diff_lookup = [lookup_times[i] - lookup_times[i-1] for i in range(1, len(lookup_times))]

# Plot erstellen
fig, axs = plt.subplots(2, 1, figsize=(10, 10))
bar_width = 0.35
index = range(len(titles))

# Erster Plot: Absolute Zeiten
axs[0].bar(index, insert_times, bar_width, label="Insert Time")
axs[0].bar([i + bar_width for i in index], lookup_times, bar_width, label="Lookup Time")
axs[0].set_xlabel("Title")
axs[0].set_ylabel("Time (seconds)")
axs[0].set_title("Insert vs Lookup Time per Title")
axs[0].set_xticks([i + bar_width / 2 for i in index])
axs[0].set_xticklabels(titles, rotation=45, ha="right")
axs[0].legend()

# Zweiter Plot: Negative Differenzen nach oben wachsen lassen
index_diff = range(1, len(titles))
axs[1].bar(index_diff, [-d for d in diff_insert], bar_width, label="Insert Time Difference")
axs[1].bar([i + bar_width for i in index_diff], [-d for d in diff_lookup], bar_width, label="Lookup Time Difference")
axs[1].set_xlabel("Title")
axs[1].set_ylabel("Negative Time Difference (seconds)")
axs[1].set_title("Insert vs Lookup Time Difference per Title (Inverted)")
axs[1].set_xticks([i + bar_width / 2 for i in index_diff])
axs[1].set_xticklabels(titles[1:], rotation=45, ha="right")
axs[1].legend()

plt.tight_layout()
plt.show()
