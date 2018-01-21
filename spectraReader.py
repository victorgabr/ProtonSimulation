# reading spectra file
from __future__ import unicode_literals
import pandas as pd
import numpy as np
import matplotlib

matplotlib.rcParams['text.usetex'] = True
import matplotlib.pyplot as plt

class SpectraReader:
    def __init__(self):
        self._n_voxels = None
        self._spectra_matrix = None
        self._n_dims = None

    @property
    def spectra_matrix(self):
        return self._spectra_matrix

    @property
    def n_voxels(self):
        return self._n_voxels

    @property
    def n_dims(self):
        return self._n_dims

    def read(self, filename):
        # todo add value checking
        dfe = pd.read_csv(filename, comment="#", header=None)
        spectra_values = dfe.values[:, 2:]
        voxel_idx = np.unique(spectra_values[:, 0])
        fluence = spectra_values[:, 1]
        self._n_dims = sum(spectra_values[:, 0] < 1)
        self._n_voxels = len(voxel_idx)
        # test reshape index
        fluence_map = fluence.reshape(self.n_voxels, self.n_dims, order="F")
        self._spectra_matrix = fluence_map
        #
        return fluence_map

    def plot_spectra(self, at, voxel_width, energy_bins):
        # plotting spectra
        Nz = self.n_voxels
        z = np.arange(Nz)
        # extent x,y,z

        zz = (-Nz + 1 + 2 * z) * voxel_width / 2
        # first dose value at voxel_z_Width / 2
        zz += zz.max() + voxel_width / 2

        # nearest index at position in mm
        idx_at = np.argmin(abs(zz - at))
        plt.plot(energy_bins, self.spectra_matrix[idx_at], label=str(zz[idx_at]) + " mm")
        plt.title("Energy spectra")
        plt.xlabel("Energy [MeV]")
        plt.legend()


if __name__ == '__main__':
    spectra_file = '/home/victor/Dropbox/victorgabr-geant4/build-SimulationBase-GEANT4_Clang-Release/PromptGammaSpectra.csv'
    spectra_reader = SpectraReader()
    spectra_reader.read(spectra_file)
    # plot proton spectra at

    bins_pairs = []

    max_energy = 10000  # keV
    bin_size = max_energy / spectra_reader.n_dims
    bin_edges = np.arange(0, max_energy + bin_size, bin_size)

    plt.style.use("ggplot")

    # test gamma histogram
    xe = bin_edges[1:] / 1000  # MeV
    spec = spectra_reader.spectra_matrix[0] / 1000000  # 10e6 protons

    fig, ax = plt.subplots()
    ax.ticklabel_format(style='sci', axis='y',scilimits=(0,0))
    ax.plot(xe, spec)
    ax.set_title(r"$\textbf{Prompt gamma-ray spectra around 175 MeV proton Bragg peak in water}$", fontsize=16)
    # ref to https://gray.mgh.harvard.edu/research/imaging/246-prompt-gamma
    ax.set_xlabel(r"$\textit{Energy}\hspace{3mm} [MeV]$", fontsize=16)
    ax.set_ylabel(r"$\textit{Fluence} \hspace{3mm}[\frac{1}{cm^2 . proton}]$", fontsize=16)
    plt.show()
