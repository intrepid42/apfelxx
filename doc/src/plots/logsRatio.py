import numpy as np
#import pylab as pl
import matplotlib.pyplot as pl

pl.style.use('seaborn-talk')
pl.rc('axes', linewidth=2)
ax = pl.gca()

ax.get_yaxis().set_tick_params(which='both', direction='in', width=2, right='on')
ax.get_xaxis().set_tick_params(which='both', direction='in', width=2, top='on')
ax.get_yaxis().set_tick_params(which='major', size=10)
ax.get_xaxis().set_tick_params(which='major', size=10)
ax.get_yaxis().set_tick_params(which='minor', size=5)
ax.get_xaxis().set_tick_params(which='minor', size=5)

data = np.loadtxt('logs.dat')

pl.title(r"Non-perturbative modified logarithms at $Q = 10$ GeV", fontsize=20)

pl.xlabel(r'$q_T$ [GeV]', fontsize=22, labelpad=-2)
pl.ylabel(r'$\widetilde{I}_1(q_T,Q)\, /\, I_1(q_T,Q)$', fontsize=22, labelpad=-2)
pl.xlim(4, 10)
pl.ylim(0.95, 1.1)
pl.xticks(size=20)
pl.yticks(size=20)

pl.plot(data[1000:2000,1], data[1000:2000,2] / data[1000:2000,3], 'r-')
pl.plot(data[1000:2000,1], data[1000:2000,3] / data[1000:2000,3], 'k-')

pl.legend(fontsize=22)

pl.savefig('logsRatio.pdf')
pl.show()
