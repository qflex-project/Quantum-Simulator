#include "lib_shor.h"
#include "dgm.h"
#include <vector>
#include <iostream>
#include <sys/time.h>
#include <map>
#include <tuple>
#include <string>

using namespace std;

int main(int argc, char** argv){
	map <int, int> qubitsMap;
	qubitsMap[11] = 15;
	qubitsMap[13] = 21;
	qubitsMap[15] = 59;
	qubitsMap[17] = 119;
	qubitsMap[19] = 253;
	qubitsMap[21] = 485;
	qubitsMap[23] = 1017;
	qubitsMap[25] = 2045;
	qubitsMap[27] = 2863;
	qubitsMap[29] = 4595; // 5 * 919
	qubitsMap[31] = 10109; // 11 * 919

	srand (time(NULL));

	struct timeval timev, tvBegin, tvEnd;
	float t;

	int n_threads = 16, cpu_region = 12, cpu_coalesc = 8, multi_gpu = 1, gpu_region = 12, gpu_coalesc = 4, tam_block = 512, rept = 4;

	int num_amostras = 1;
	int qubits = 31;
	int execType = t_HYBRID_2;
	int qubits_limit = 23;
	int global_coalesc = 26;

	if (argc > 1) {
		qubits = atoi(argv[1]);
	}

	if (qubitsMap.count(qubits) == 0){
		cout << "The amount of qubits does not map to a valid number to be factored: " << qubits << endl; 
		return 0;
	}

	if (argc > 2) {
		execType = atoi(argv[2]);
	}
 
	if (argc > 3) {
		qubits_limit = atoi(argv[3]);
	}

	if (argc > 4) {
		global_coalesc = atoi(argv[4]);
	}

	vector < tuple <int, int, vector <float> > > results;

	vector <int> qubits_limit_values = { qubits_limit };
	vector <int> global_coales_values = { global_coalesc };

	string execTypeStr = std::to_string(execType);
	if (execType == t_HYBRID) {
		execTypeStr = "Hybrid";
	}
	else if (execType == t_HYBRID_2) {
		execTypeStr = "Hybrid 2";
	}
	else if (execType == t_GPU_2) {
		execTypeStr = "GPU 2";
	}
	cout << "Exec Type: " << execTypeStr << endl;

	for (auto& qubits_limit: qubits_limit_values) {
	for (auto& global_coales: global_coales_values) {
		if (global_coales >= (qubits_limit - 2))
			continue;

		//cout << "Qubits Limit: " << qubits_limit << "  -  Global Coales: " << global_coales << endl;
		vector <float> amostras;
		for (int a = 0; a < num_amostras; a++) {
			gettimeofday(&tvBegin, NULL);
			Shor(qubitsMap[qubits], execType, n_threads, cpu_region, cpu_coalesc, 1, gpu_region, gpu_coalesc, tam_block, rept, qubits_limit, global_coales);
			gettimeofday(&tvEnd, NULL);
			timeval_subtract(&timev, &tvEnd, &tvBegin);
			t = timev.tv_sec + (timev.tv_usec / 1000000.0);

			//cout << a << ": " << t << endl;

			amostras.push_back(t);
		}

		tuple <int, int, vector <float>> result;

		result = make_tuple(qubits_limit, global_coales, amostras);

		results.push_back(result);
	}
	}

	/*
	cout << "\nALL RESULTS" << endl;

	for (auto& result: results) {
		cout << get<0>(result) << " " << get<1>(result);
		vector<float> amostras = get<2>(result);
		for (int a = 0; a < num_amostras; a++) {
			cout << " " << amostras[a];
		}
		cout << endl;
	}

	cout << "-------------------------------" << endl;
	*/

	/*
	if (argc < 2){
		cout << "You need to define the execution parameters" << endl;
		return 0;
	}

	int qubits = atoi(argv[1]);
	if (qubitsMap.count(qubits) == 0){
		cout << "The amount of qubits does not map to a valid number to be factored: " << qubits << endl; 
		return 0;
	}
 
	if (argc > 2) {
		execType = atoi(argv[2]);
	}

	if (execType < t_CPU || execType > t_HYBRID_2){
		cout << "Invalid execution type: " << execType << endl; 
		return 0;
	}

	if (execType == t_PAR_CPU) {
		if (argc > 3) n_threads = atoi(argv[3]);
	}
	else if (execType == t_GPU) {
		if (argc > 3) multi_gpu = atoi(argv[3]);
	}
	else if (execType == t_HYBRID) {
		if (argc > 3) n_threads = atoi(argv[3]);
	}

	vector<int> factors;

	cout << "Executing Shor: " << qubits << " qubits" << endl;

	gettimeofday(&tvBegin, NULL);
	factors = Shor(qubitsMap[qubits], execType, n_threads, cpu_region, cpu_coalesc, multi_gpu, gpu_region, gpu_coalesc, tam_block, rept);
	gettimeofday(&tvEnd, NULL);
	timeval_subtract(&timev, &tvEnd, &tvBegin);
	t = timev.tv_sec + (timev.tv_usec / 1000000.0);
	
	cout << "Time: " << t << endl;

	if (factors.size() == 2) {
		cout << "Found factors: " << factors[0] << " -- " << factors[1] << endl;
	}
	else {
		cout << "Failed to find factors" << endl;
	}
	*/

	return 0;
}
