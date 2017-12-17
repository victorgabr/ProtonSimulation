# -*- coding: utf-8 -*-


import pandas as pd
import matplotlib.pyplot as plt
import subprocess
import os
import sys
import re

plt.style.use('ggplot')

if len(sys.argv) != 4:
    print("Usage: %s geometry.gdml output.csv run.mac" % sys.argv[0])
    sys.exit(1)

args = ['./ProtonSimulation', sys.argv[1], sys.argv[2], sys.argv[3]]

print(" Starting GEANT4 simulation")
subprocess.call(args)

print("Reading and ploting output file %s " % sys.argv[2])

df = pd.read_csv(sys.argv[2])
# getting source energy
energy = ''
with open(sys.argv[3], 'r') as f:
    txt = f.readlines()

for l in txt:
    if re.findall('mono', l):
        energy = l.split('mono')[-1]


# preparing plot
df['Dose [%]'] = df['Edep'] / df['Edep'].max() * 100
df['Depth [mm]'] = df['z']
df = df.set_index('Depth [mm]')

fig, ax = plt.subplots()
df['Dose [%]'].plot(ax=ax, label=energy)

ax.set_title("%s - Pristine Bragg Peak" % sys.argv[1])
ax.legend()
ax.set_ylabel('Dose [%]')
ax.set_ylim([0,140])
# saving figures
fig_name = "%s - %s - Pristine Bragg Peak" % (sys.argv[1], energy)
fig.savefig(fig_name, format='png', dpi=100)
plt.show()
