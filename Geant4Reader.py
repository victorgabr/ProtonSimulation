import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

import sys
import subprocess

# plt.style.use('ggplot')




#
# if len(sys.argv) != 2:
#     print("Usage: %s geometry.gdml" % sys.argv[0])
#     sys.exit(1)
# #
# args = ['./SimulationBase', sys.argv[1]]
# #
# print(" Starting GEANT4 simulation")

# subprocess.call(args)

# testing new output

# test numpy indexing
# arq1 = '/home/victor/Dropbox/victorgabr-geant4/build-SimulationBase-GEANT4_Clang-Release/EnergyDeposition_Proton_100MeV.out'

#
arq = '/home/victor/Dropbox/victorgabr-geant4/build-SimulationBase-GEANT4_Clang-Release/EnergyDeposition_Proton_100MeV.csv'

# GETTING scoring sizes
with open(arq) as f:
    txt = f.readlines()
    size = np.asarray(eval(txt[2].split(":")[-1]))
    voxels = np.asarray(eval(txt[3].split(":")[-1]))

numberOfVoxel_x = fNx = voxels[0]
numberOfVoxel_y = fNy = voxels[1]
numberOfVoxel_z = fNz = voxels[2]

voxelWidth = (size * 2) / voxels

print("Size Total xyz [mm]: ", size * 2)
print("Voxels: ", voxels)
print("voxelWidth: ", voxelWidth)

# reading output file
df = pd.read_csv(arq, skiprows=5, header=None)
vals = df.values
tmp = np.zeros((fNx, fNy, fNz))
for row in vals:
    r, c, d = row[:3].astype(int)
    tmp[r, c, d] = row[3]

dose_map = tmp.copy()

# calculating coordinates
x = np.arange(numberOfVoxel_x)
y = np.arange(numberOfVoxel_y)
z = np.arange(numberOfVoxel_z)
# extent x,y,z
xx = (-numberOfVoxel_x + 1 + 2 * x) * voxelWidth[0] / 2
yy = (-numberOfVoxel_y + 1 + 2 * y) * voxelWidth[1] / 2
zz = (-numberOfVoxel_z + 1 + 2 * z) * voxelWidth[2] / 2
# first dose value at voxel_z_Width / 2
zz += zz.max() + voxelWidth[2] / 2

# plotting values
plt.set_cmap("nipy_spectral")
# XY plane - axial
z_idx = 0
im = dose_map[:, :, z_idx]
plt.imshow(im, interpolation="bicubic", extent=[xx.min(), xx.max(), yy.min(), yy.max()])
plt.xlabel("x [mm]")
plt.ylabel("y [mm]")
plt.title("Depth z %s mm" % str(zz[z_idx]))

# plotting values
# xy plane
plt.figure()
y_idx = int(numberOfVoxel_y / 2)
x_proj = dose_map.sum(axis=0)

plt.imshow(x_proj, interpolation="bicubic", extent=[zz.min(), zz.max(), xx.min(), xx.max()])
plt.xlabel("z [mm]")
plt.ylabel("x [mm]")
plt.title("X-axis projection")

# plotting values
# zx plane
plt.figure()
x_idx = int(numberOfVoxel_x / 2)
x_proj = dose_map.sum(axis=1)
plt.imshow(x_proj, interpolation="bicubic", extent=[zz.min(), zz.max(), yy.min(), yy.max()])
plt.xlabel("z [mm]")
plt.ylabel("y [mm]")
# plt.title("x  %s mm" % str(xx[x_idx]))
plt.title("Y-axis projection")

# plot bragg curve
bragg_curve = dose_map.sum(axis=1).sum(axis=0)
bragg_curve /= bragg_curve.max()
bragg_curve *= 100
plt.figure()
plt.plot(zz, bragg_curve)
plt.xlabel("Z-Depth [mm]")
plt.ylabel("PDD(%)")
plt.title("Bragg curve")
plt.show()
