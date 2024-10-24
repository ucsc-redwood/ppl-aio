import yaml
import matplotlib.pyplot as plt
import os

# Define the project root directory
project_root = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))

# Set the paths for the YAML file and output image
yaml_file = os.path.join(project_root, 'data', 'phones_config.yaml')
output_file = os.path.join(project_root, 'data', 'core_composition.png')  

# Create the output directory if it doesn't exist
os.makedirs(os.path.dirname(output_file), exist_ok=True)

# Load the YAML file
with open(yaml_file, 'r') as file:
    data = yaml.safe_load(file)

# Prepare data for plotting
phones = []
small_cores_count = []
mid_cores_count = []
big_cores_count = []

for uid, config in data['phones'].items():
    phones.append(config['alias'])
    small_cores_count.append(len(config.get('small_cores', [])))
    mid_cores_count.append(len(config.get('mid_cores', [])))
    big_cores_count.append(len(config.get('big_cores', [])))

# Create a stacked bar chart
fig, ax = plt.subplots()

width = 0.5  # Width of the bars
p1 = ax.bar(phones, small_cores_count, width, label='Small Cores')
p2 = ax.bar(phones, mid_cores_count, width, bottom=small_cores_count, label='Mid Cores')
p3 = ax.bar(phones, big_cores_count, width, bottom=[i+j for i,j in zip(small_cores_count, mid_cores_count)], label='Big Cores')

# Add some text for labels, title, and custom x-axis tick labels
ax.set_xlabel('Phones')
ax.set_ylabel('Number of Cores')
ax.set_title('Core Composition of Phones (Small, Mid, Big)')
ax.legend()

# Save the plot to the specified file instead of showing it
plt.xticks(rotation=45, ha='right')
plt.tight_layout()
# plt.show()
plt.savefig(output_file)

print(f"Graph saved to {output_file}")
