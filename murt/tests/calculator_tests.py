from murt import calculator


txPos = [float(1.0), float(2.0), float(3.0)]
rxPos = [20, 5, 10]
refPos = [-10, 3, 30]

txFreq = float(2.4e9)
a = calculator.directLoss(txPos, rxPos, txFreq)
print(f'tx:{txPos}, rx:{rxPos}, freq: {txFreq}, pl = {a}')


b = calculator.reflectLoss(txPos, rxPos, refPos, txFreq, 5.5)
print(f'tx:{txPos}, rx:{rxPos}, ref: {refPos}, freq: {txFreq}, pl = {b}')
