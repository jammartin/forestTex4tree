import h5py
import numpy as np

if __name__ == '__main__':

    particles = [(4.7, 1.0, 2.0),
                 (0.5, 2.1, 3.1),
                 (1.4, 13.1, 0.5),
                 (0.6, 0.1, 10.5),
                 (10.3, 5.6, 1.4),
                 (7.5, 8.5, 1.4),
                 (1.3, 10.9, 9.5),
                 (5.4, 6.4, 6.4)]

    hf = h5py.File('particles.h5', 'w')

    hf.create_dataset('x', data=np.array(particles))

    hf.close()

