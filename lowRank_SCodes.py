import numpy as np
import json, os, argparse

parser = argparse.ArgumentParser(description="Liveness inference")
parser.add_argument(
    "-t", "--time_points", type=int, default=4, help="Total time points",
)
parser.add_argument(
    "-i", "--input_path", type=str, required=True, help="Input matrix path",
)
parser.add_argument(
    "-o", "--output_path", type=str, required=True, help="output matirx path"
)
parser.add_argument(
    "-d", "--debug", action="store_true", help="debug mode only"
)
parser.add_argument(
    "-r1", "--lambda1", type=float, default=0.01, help="labmda1",
)
parser.add_argument(
    "-r2", "--lambda2", type=float, default=0.001, help="labmda2",
)
parser.add_argument(
    "-r3", "--lambda3", type=float, default=0.0005, help="lambda3",
)


args = parser.parse_args()

def lowRank_sparse(SMatrix):
	u, s, vh = np.linalg.svd(SMatrix, full_matrices=True)
	news = news.shape[0]
	u = u[:, :news] * s
	output = u * vh

	return output

def resemble_sparse(SMatrices):
	outputS = []
	for p in range(1, args.time_points-1):
		output = np.zeros(Smatrices[0].shape)
		for q in range(p+1, args.time_points):
			output += (1/(p-q)) * np.sqare(SMatrices[p] - Smatrices[q])
		outputS.append(output)
	return outputS

if __name__ == "__main__":

	Sall = []
	for T in range(args.time_points):
		sfile = os.path.join(args.input_path, 'SparseCodes%d.txt'%T)
		fin = open(sfile, 'r')
		inputS = fin.readlines()
		inputS = np.array(inputS)
		outputS = lowRank_sparse(inputS)
		Sall.append(outputS)
		fin.close()

	diffS = resemble_sparse(outputS)
	for T in range(len(diffS)):
		outputS[T] = args.lambda2/args.lambda1 * outputS[T] + args.lambda3/args.lambda1 * diffS[T]
		
		row_sums = outputS[T].sum(axis=1)
		for i, num in enumerate(row_sums):
		 	if num == 0:
		 		row_sums[i] = 1e-10
		outputS[T] = outputS[T] / row_sums[:, numpy.newaxis]

		outfile = os.path.join(args.output_path, 'SparseCodes%d_new.txt'%T)
		fout = open(outfile, 'w')
		for row in outputS[T]:
			np.savetxt(fout, row)
