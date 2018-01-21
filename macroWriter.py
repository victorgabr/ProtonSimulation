"""
Copyright (c) 2018 Victor Gabriel Leandro Alves.

This file is part of ProtonSimulation GEANT4 code

"""

import numpy as np


class SpectraMacroWriter:
    def __init__(self, particle="proton",
                 min_energy=0,
                 max_energy=100,
                 bin_size=1,
                 quantity="MeV",
                 box_size=(15, 25, 175),
                 n_voxels=(1, 1, 175),
                 translate_xyz=(0., 0., 0.)):
        """
            Helper class to write particle spectra scoring macro
            GEANT4
        :param particle: Particle type (eg. proton, e-, e+, gamma, etc)
        :param quantity: Energy quantity (MeV)
        :param max_energy: Maximum energy
        :param bin_size: Histogram bin size
        :param box_size: Scoring Mesh in mm (x,y,z) (half size)
        :param n_voxels: Number of voxels per dimension (nx,ny,nz) (half size)
        :param translate_xyz: x,y,z coordinates translation in mm
        """
        # TODO check parameters
        self.min_energy = min_energy
        self.particle = particle
        self.quantity = quantity
        self.bin_size = bin_size
        self.max_energy = max_energy
        self._header = ""
        self._body = ""
        self._footer = "/score/close\n" \
                       "/score/list"""

        # setters
        self.header = (box_size, n_voxels, translate_xyz)
        self.body = (max_energy, bin_size)

    @property
    def body(self):
        return self._body

    @body.setter
    def body(self, value):
        # todo add value checking
        max_energy = value[0]
        bin_size = value[1]
        bin_edges = np.arange(self.min_energy, max_energy + bin_size, bin_size)

        bins_pairs = []
        for i in range(len(bin_edges) - 1):
            bins_pairs.append((round(bin_edges[i]), round(bin_edges[i + 1])))

        body = ""
        for i in range(len(bins_pairs)):
            body += self.get_bin_txt(i, bins_pairs[i][0], bins_pairs[i][1], self.quantity, self.particle)

        self._body = body

    @property
    def header(self):
        return self._header

    @header.setter
    def header(self, value):

        box_size = value[0]
        n_voxels = value[1]
        translate_xyz = value[2]
        self._header = "#Defines a detector using a boxMesh command\n" \
                       "/score/create/boxMesh spectraDetector\n" \
                       "/score/mesh/boxSize %1.1f %1.1f %1.1f mm\n" \
                       "/score/mesh/nBin %i %i %i\n" \
                       "/score/mesh/translate/xyz %1.1f %1.1f %1.1f  mm\n" \
                       "# scoring spectra\n" % (box_size[0],
                                                box_size[1],
                                                box_size[2],
                                                n_voxels[0],
                                                n_voxels[1],
                                                n_voxels[2],
                                                translate_xyz[0],
                                                translate_xyz[1],
                                                translate_xyz[2])

    @property
    def footer(self):
        return self._footer

    def write(self, filename="Output.mac"):
        with open(filename, "w") as f:
            f.write(self.header)
            f.write(self.body)
            f.write(self.footer)

    @staticmethod
    def get_bin_txt(idx, bin_min, bin_max, quantity_string, particle_string):
        """
            Helper method to return bin txt to GEANT4 macro file
            # quantity_string = "MeV"
            # particle_string = "proton"

        :param idx: bin index
        :param bin_min: bin lower bound
        :param bin_max: bin upper bound
        :param quantity_string: str ("MeV", "keV", etc)
        :param particle_string: str ("proton", "gamma", "e-", etc)
        :return: bin txt

        example output:
            # scoring spectra
            /score/quantity/flatSurfaceFlux flux1
            /score/filter/particleWithKineticEnergy range1 1. 2.5 MeV proton
            /score/quantity/flatSurfaceFlux flux2
            /score/filter/particleWithKineticEnergy range2 2.5 5.0 MeV proton
        """
        scorer_string = "/score/quantity/flatSurfaceFlux"
        filter_string = "/score/filter/particleWithKineticEnergy"

        write_bin = "%s fux%i \n" \
                    "%s range%i %1.1f %1.1f %s %s\n" % (scorer_string,
                                                        idx,
                                                        filter_string,
                                                        idx,
                                                        round(bin_min, 1),
                                                        round(bin_max, 1),
                                                        quantity_string,
                                                        particle_string)

        return write_bin


if __name__ == '__main__':
    writerObj = SpectraMacroWriter(particle="gamma",
                                   max_energy=10000,
                                   quantity="keV",
                                   bin_size=10,
                                   box_size=(200, 5, 20),
                                   n_voxels=(1, 1, 1),
                                   translate_xyz=(0, -150, 0))
    writerObj.write()
