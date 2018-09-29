def empiricalFormula(molarMasses, args):
	if sum(args) != 100:
		print('sum: ' + str(sum(args)))
	mols = []
	for arg, mm in zip(args, molarMasses):
		mols.append(arg/mm)
	ratios = [m/min(mols) for m in mols]
	return ratios
    
def dot(x,y):
	if len(x) != len(y):
		print('invalid args - diff lengths')
	sum = 0
	for i in range(len(x)):
		sum += x[i] * y[i]
	return sum

def substanceRatio(molarmass, molarmasses, ratios):
	r = empiricalFormula(molarmasses, ratios)
	return [x * molarmass / dot(molarmasses,r) for x in r]

# Molar Masses of substances
C = 12.0107
H = 1.00794
O = 15.9994
Cl = 35.453
Fe = 55.845
P = 30.973761
S = 32.065
N = 14.0067
F = 18.9984032
I = 126.90447
