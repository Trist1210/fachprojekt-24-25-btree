#!/usr/bin/env python3


import pandas as pd
import seaborn as sns
import sys
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
    print("Usage: python plot.py <input_file>")
    sys.exit(1)

input_file = sys.argv[1]
data = pd.read_csv(input_file)


sns.barplot(x='counter', y='value', hue='additional', data=data)

# Add labels and title
plt.xlabel('Counter')
plt.ylabel('Value')
plt.title('Comparison of Counter Before and After')
plt.legend()

# Better readability
plt.xticks(rotation=45)
plt.tight_layout()

# Save the plot to a file
plt.savefig('comparison_plot.pdf')
