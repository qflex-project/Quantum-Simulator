#ifndef _DGM_H_
#define _DGM_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <algorithm>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "common.h"
#include "gates.h"
#include "pt.h"

#define complex _Complex

using namespace std;

float complex *GenericExecute(float complex *state, string function, int qubits,
                              int type, int threads, int factor);
float complex *GenericExecute(float complex *state, vector<string> function,
                              int qubits, int type, int threads, int factor);

void PCpuExecution1(float complex *state, PT **pts, int qubits, long n_threads,
                    int coales, int region, int it);
void PCpuExecution1_0(float complex *state, PT **pts, int qubits, int start,
                      int end, int pos_count, int reg_id, int reg_mask);

inline long LINE(long pos, long shift) { return ((pos >> shift) & 1) * 2; }
inline long BASE(long pos, long shift) { return pos & (~(1 << shift)); }

enum { t_CPU, t_PAR_CPU, t_GPU, t_HYBRID, t_SPEC };

class Group {
 public:
  vector<string> ops;
  vector<long> pos_ops;
  vector<bool> ctrl;
  vector<long> pos_ctrl;

  Group(){};
  bool isAfected(int pos, int afect);
};

class DGM {
 public:
  long total_op, dense, main_diag, sec_diag, c_dense, c_main_diag,
      c_sec_diag;  // counters

  vector<string> diag;
  long MAX_QB, MAX_PT, qb_afected;

  long factor;

  int exec_type;

  long n_threads;
  long cpu_coales;
  long cpu_region;

  int multi_gpu;
  long gpu_coales;
  long gpu_region;

  int tam_block;
  int rept;

  vector<PT *> vec_pts;
  PT **pts;
  long qubits;

  float measure_value;

  float elapsed_time;
  struct timeval timev;

  float complex *state;

  DGM();
  ~DGM();

  bool en_print;

  void printPTs();
  void erase();
  void setExecType(int type);

  void setCpuStructure(long cpu_region, long cpu_coales);
  void setGpuStructure(long gpu_coales, long gpu_region, int rept = 1);

  void allocateMemory();
  void setMemory(float complex *mem);
  void freeMemory();
  void setMemoryValue(int pos);

  int measure(int q_pos);
  map<long, float> measure(vector<int> q_pos);
  void colapse(int q_pos, int value);

  void setFunction(string function, int it = 1, bool er = true);
  void setFunction(vector<string> steps, int it = 1, bool er = true);
  map<long, Group> genGroups(string step);
  void genPTs(map<long, Group> &gps, vector<PT *> &step_pts);
  void genMatrix(float complex *matrix, vector<float complex *> &matrices,
                 long tam, long current, long line, long column,
                 float complex cmplx);

  void CountOps(int it = 1);

  void executeFunction(string function, int it = 1);
  void executeFunction(vector<string> steps, int it = 1);
  float complex *execute(int it);

  void HybridExecution(PT **pts);

  void CpuExecution1(int it);
  void CpuExecution1_1(PT *pt, long mem_size);
  void CpuExecution1_2(PT *pt, long mem_size);
  void CpuExecution1_3(PT *pt, long mem_size);

  // void CpuExecution1(int it);
  void CpuExecution2_1(PT *pt, long mem_size);
  void CpuExecution2_2(PT *pt, long mem_size);
  void CpuExecution2_3(PT *pt, long mem_size);

  void CpuExecution3_1(PT *pt, long mem_size);
  void CpuExecution3_2(PT *pt, long mem_size);
  void CpuExecution3_3(PT *pt, long mem_size);
};

#endif
