!original->
#Workload: insert phase: 50000000 inserts / mixed phase: 50000000 requests
Insert phase[0]: seconds: 19.9809 | instructions: 3.71755e+10 | cycles: 1.00826e+11 | 
Lookup phase[0]: seconds: 20.8922 | instructions: 2.53572e+10 | cycles: 1.05475e+11 | 
Insert phase[1]: seconds: 20.1702 | instructions: 3.63848e+10 | cycles: 1.0183e+11 | 
Lookup phase[1]: seconds: 21.1449 | instructions: 2.53627e+10 | cycles: 1.06708e+11 | 
Insert phase[2]: seconds: 20.5065 | instructions: 3.63378e+10 | cycles: 1.03477e+11 | 
Lookup phase[2]: seconds: 21.6252 | instructions: 2.53584e+10 | cycles: 1.09107e+11 | 
Insert phase[3]: seconds: 20.7631 | instructions: 3.63318e+10 | cycles: 1.04781e+11 | 
Lookup phase[3]: seconds: 21.7573 | instructions: 2.53414e+10 | cycles: 1.09714e+11 | 
Insert phase[4]: seconds: 20.9965 | instructions: 3.63111e+10 | cycles: 1.05944e+11 | 
Lookup phase[4]: seconds: 22.2882 | instructions: 2.53888e+10 | cycles: 1.1235e+11 | 

!multi-threading->
#Workload: insert phase: 50000000 inserts / mixed phase: 50000000 requests
Insert phase[0]: seconds: 1.66709 | instructions: 2.33342e+09 | cycles: 7.67169e+09 | 
Lookup phase[0]: seconds: 1.5362 | instructions: 1.58378e+09 | cycles: 7.10919e+09 | 
Insert phase[1]: seconds: 1.72053 | instructions: 2.28138e+09 | cycles: 7.85335e+09 | 
Lookup phase[1]: seconds: 1.51488 | instructions: 1.58424e+09 | cycles: 7.09196e+09 | 
Insert phase[2]: seconds: 1.76719 | instructions: 2.27872e+09 | cycles: 7.85372e+09 | 
Lookup phase[2]: seconds: 1.57214 | instructions: 1.5825e+09 | cycles: 7.40775e+09 | 
Insert phase[3]: seconds: 1.7208 | instructions: 2.27033e+09 | cycles: 7.90098e+09 | 
Lookup phase[3]: seconds: 1.59043 | instructions: 1.58455e+09 | cycles: 7.32851e+09 | 
Insert phase[4]: seconds: 1.71946 | instructions: 2.27546e+09 | cycles: 8.01651e+09 | 
Lookup phase[4]: seconds: 1.63108 | instructions: 1.58446e+09 | cycles: 7.35926e+09 | 

! 20 element-batching->
#Workload: insert phase: 50000000 inserts / mixed phase: 50000000 requests
Insert phase[0]: seconds: 1.65224 | instructions: 2.32924e+09 | cycles: 7.49913e+09 | 
Lookup phase[0]: seconds: 0.854189 | instructions: 1.97085e+09 | cycles: 3.95203e+09 | 
Insert phase[1]: seconds: 1.71955 | instructions: 2.27463e+09 | cycles: 7.89289e+09 | 
Lookup phase[1]: seconds: 0.773648 | instructions: 1.97131e+09 | cycles: 3.67334e+09 | 
Insert phase[2]: seconds: 1.75099 | instructions: 2.27866e+09 | cycles: 7.87923e+09 | 
Lookup phase[2]: seconds: 0.842 | instructions: 1.97134e+09 | cycles: 3.61458e+09 | 
Insert phase[3]: seconds: 1.77694 | instructions: 2.2737e+09 | cycles: 7.95294e+09 | 
Lookup phase[3]: seconds: 0.818531 | instructions: 1.9718e+09 | cycles: 3.56873e+09 | 
Insert phase[4]: seconds: 1.73575 | instructions: 2.26886e+09 | cycles: 7.93354e+09 | 
Lookup phase[4]: seconds: 0.750348 | instructions: 1.96979e+09 | cycles: 3.59778e+09 | 

!167U Simd->
#Workload: insert phase: 50000000 inserts / mixed phase: 50000000 requests
Insert phase[0]: seconds: 1.91778 | instructions: 2.6312e+09 | cycles: 7.91143e+09 | 
Lookup phase[0]: seconds: 0.717004 | instructions: 2.25669e+09 | cycles: 3.22922e+09 | 
Insert phase[1]: seconds: 2.00714 | instructions: 2.63135e+09 | cycles: 8.38438e+09 | 
Lookup phase[1]: seconds: 0.655937 | instructions: 2.2549e+09 | cycles: 3.07289e+09 | 
Insert phase[2]: seconds: 1.86611 | instructions: 2.62495e+09 | cycles: 8.21019e+09 | 
Lookup phase[2]: seconds: 0.633376 | instructions: 2.25532e+09 | cycles: 2.97094e+09 | 
Insert phase[3]: seconds: 1.88379 | instructions: 2.6276e+09 | cycles: 8.24057e+09 | 
Lookup phase[3]: seconds: 0.676726 | instructions: 2.25514e+09 | cycles: 3.10859e+09 | 
Insert phase[4]: seconds: 1.85477 | instructions: 2.6168e+09 | cycles: 8.17225e+09 | 
Lookup phase[4]: seconds: 0.665734 | instructions: 2.25498e+09 | cycles: 3.00674e+09 | 

! vertauschen->
#Workload: insert phase: 50000000 inserts / mixed phase: 50000000 requests
Insert phase[0]: seconds: 1.88174 | instructions: 2.65847e+09 | cycles: 8.1457e+09 | 
Lookup phase[0]: seconds: 0.605768 | instructions: 2.25503e+09 | cycles: 2.65818e+09 | 
Insert phase[1]: seconds: 1.84133 | instructions: 2.62737e+09 | cycles: 8.11126e+09 | 
Lookup phase[1]: seconds: 0.569677 | instructions: 2.25549e+09 | cycles: 2.6899e+09 | 
Insert phase[2]: seconds: 1.78949 | instructions: 2.62689e+09 | cycles: 8.18272e+09 | 
Lookup phase[2]: seconds: 0.607297 | instructions: 2.25255e+09 | cycles: 2.6173e+09 | 
Insert phase[3]: seconds: 1.84558 | instructions: 2.62007e+09 | cycles: 8.16958e+09 | 
Lookup phase[3]: seconds: 0.603136 | instructions: 2.25565e+09 | cycles: 2.81346e+09 | 
Insert phase[4]: seconds: 1.8255 | instructions: 2.6235e+09 | cycles: 8.06865e+09 | 
Lookup phase[4]: seconds: 0.561413 | instructions: 2.25582e+09 | cycles: 2.63876e+09 | 

! 550U
#Workload: insert phase: 50000000 inserts / mixed phase: 50000000 requests
Insert phase[0]: seconds: 1.38311 | instructions: 1.87589e+09 | cycles: 6.28386e+09 | 
Lookup phase[0]: seconds: 0.630153 | instructions: 1.71758e+09 | cycles: 2.92094e+09 | 
Insert phase[1]: seconds: 1.37831 | instructions: 1.74062e+09 | cycles: 6.5171e+09 | 
Lookup phase[1]: seconds: 0.651303 | instructions: 1.6883e+09 | cycles: 2.97985e+09 | 
Insert phase[2]: seconds: 1.4557 | instructions: 1.7508e+09 | cycles: 6.6496e+09 | 
Lookup phase[2]: seconds: 0.652738 | instructions: 1.69305e+09 | cycles: 2.81231e+09 | 
Insert phase[3]: seconds: 1.43818 | instructions: 1.76092e+09 | cycles: 6.68806e+09 | 
Lookup phase[3]: seconds: 0.65213 | instructions: 1.68969e+09 | cycles: 3.08379e+09 | 
Insert phase[4]: seconds: 1.40117 | instructions: 1.74247e+09 | cycles: 6.69449e+09 | 
Lookup phase[4]: seconds: 0.664981 | instructions: 1.69997e+09 | cycles: 2.9672e+09 | 

