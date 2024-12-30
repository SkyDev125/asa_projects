import os
import subprocess
import json
import csv
import natsort
from decimal import Decimal

# Create the spreadsheets directory if it does not exist
os.makedirs('spreadsheets', exist_ok=True)

# Clean the spreadsheets directory
for file in os.listdir('spreadsheets'):
    os.remove(os.path.join('spreadsheets', file))

# Custom JSON decoder to use Decimal for floating-point numbers
def decimal_decoder(obj):
    if isinstance(obj, float):
        return Decimal(str(obj))
    return obj

# Iterate over each folder
for folder in natsort.natsorted(os.listdir('.')):
    if folder == 'spreadsheets' or not os.path.isdir(folder):
        continue

    # Create a CSV file for the current folder
    csv_file = f"spreadsheets/{folder}.csv"
    with open(csv_file, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(["stations", "connections", "lines", "mean", "stddev", "median", "user", "system", "min", "max"])

        # Iterate over each JSON file in the folder
        for input_json in natsort.natsorted(os.listdir(folder)):
            if input_json.endswith('.analysis.json'):
                input_json_path = os.path.join(folder, input_json)
                with open(input_json_path, 'r') as f:
                    data = json.load(f, parse_float=decimal_decoder)

                # Extract firstnum and secondnum from the file name
                filename = os.path.basename(input_json)
                stations = filename.split('_')[1]
                connections = filename.split('_')[2]
                lines = filename.split("_")[3].split('.')[0]

                # Process JSON and append to the CSV file
                for result in data['results']:
                    csvwriter.writerow([
                        stations,
                        connections,
                        lines,
                        result['mean'],
                        result['stddev'],
                        result['median'],
                        result['user'],
                        result['system'],
                        result['min'],
                        result['max']
                    ])
            else:
                print(f"Warning: {input_json} not found, skipping...")