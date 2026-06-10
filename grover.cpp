#include "lib_grover.h"
#include "dgm.h"
#include <vector>
#include <iostream>
#include <sys/time.h>

using namespace std;

int main(int argc, char **argv){
	srand(time(NULL));

	int n_threads = 16, cpu_region = 12, cpu_coalesc = 8, multi_gpu = 1, gpu_region = 12, gpu_coalesc = 4, tam_block = 512, rept = 4;

	int num_amostras = 5;
	int qubits = 31;
	int execType = t_HYBRID_2;
 
	int value = 10;

	vector < tuple <int, int, vector <float> > > results;

	vector <int> qubits_limit_values = { 23 };
	vector <int> global_coales_values = { 16 };

	for (auto& qubits_limit: qubits_limit_values) {
	for (auto& global_coales: global_coales_values) {
		if (global_coales >= qubits_limit)
			continue;

		cout << "Qubits Limit: " << qubits_limit << "  -  Global Coales: " << global_coales << endl;
		vector <float> amostras;
		for (int a = 0; a < num_amostras; a++) {
			float t = Grover(qubits, value, execType, n_threads, cpu_region, cpu_coalesc, multi_gpu, gpu_region, gpu_coalesc, tam_block, rept, qubits_limit, global_coales);
			//Shor(qubitsMap[qubits], execType, n_threads, cpu_region, cpu_coalesc, 1, gpu_region, gpu_coalesc, tam_block, rept, qubits_limit, global_coales);

			cout << a << ": " << t << endl;

			amostras.push_back(t);
		}

		tuple <int, int, vector <float>> result;

		result = make_tuple(qubits_limit, global_coales, amostras);

		results.push_back(result);
	}
	}

	cout << "\nALL RESULTS" << endl;

	for (auto& result: results) {
		cout << get<0>(result) << " " << get<1>(result);
		vector<float> amostras = get<2>(result);
		for (int a = 0; a < num_amostras; a++) {
			cout << " " << amostras[a];
		}
		cout << endl;
	}	
}
