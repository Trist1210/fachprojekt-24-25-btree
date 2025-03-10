!original
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

!multi-threading
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

!16 Elem. batching
#Workload: insert phase: 50000000 inserts / mixed phase: 50000000 requests
Insert phase[0]: seconds: 19.9292 | instructions: 3.71774e+10 | cycles: 1.00534e+11 | 
Lookup phase[0]: seconds: 8.0343 | instructions: 3.15193e+10 | cycles: 4.05399e+10 | 
Insert phase[1]: seconds: 20.4442 | instructions: 3.63812e+10 | cycles: 1.0311e+11 | 
Lookup phase[1]: seconds: 8.10665 | instructions: 3.15353e+10 | cycles: 4.09166e+10 | 
Insert phase[2]: seconds: 20.7049 | instructions: 3.63472e+10 | cycles: 1.04467e+11 | 
Lookup phase[2]: seconds: 8.2278 | instructions: 3.15325e+10 | cycles: 4.14956e+10 | 
Insert phase[3]: seconds: 20.9129 | instructions: 3.6331e+10 | cycles: 1.05553e+11 | 
Lookup phase[3]: seconds: 8.26149 | instructions: 3.15343e+10 | cycles: 4.16865e+10 | 
Insert phase[4]: seconds: 21.0875 | instructions: 3.63292e+10 | cycles: 1.06467e+11 | 
Lookup phase[4]: seconds: 8.2617 | instructions: 3.15395e+10 | cycles: 4.17103e+10 | 

!inner simd 
#Workload: insert phase: 50000000 inserts / mixed phase: 50000000 requests
Insert phase[0]: seconds: 17.4825 | instructions: 3.08536e+10 | cycles: 8.82194e+10 | 
Lookup phase[0]: seconds: 11.2817 | instructions: 1.98916e+10 | cycles: 5.69266e+10 | 
Insert phase[1]: seconds: 18.4411 | instructions: 3.00743e+10 | cycles: 9.2995e+10 | 
Lookup phase[1]: seconds: 11.5631 | instructions: 1.98965e+10 | cycles: 5.83505e+10 | 
Insert phase[2]: seconds: 18.683 | instructions: 3.00591e+10 | cycles: 9.43216e+10 | 
Lookup phase[2]: seconds: 11.7715 | instructions: 1.98856e+10 | cycles: 5.93982e+10 | 
Insert phase[3]: seconds: 18.951 | instructions: 3.00436e+10 | cycles: 9.56588e+10 | 
Lookup phase[3]: seconds: 11.9801 | instructions: 1.99092e+10 | cycles: 6.04872e+10 | 
Insert phase[4]: seconds: 19.3277 | instructions: 3.00461e+10 | cycles: 9.75748e+10 | 
Lookup phase[4]: seconds: 11.9387 | instructions: 1.99043e+10 | cycles: 6.02682e+10 | 

!vertauscht
#Workload: insert phase: 50000000 inserts / mixed phase: 50000000 requests
Insert phase[0]: seconds: 18.3883 | instructions: 3.66952e+10 | cycles: 9.27687e+10 | 
Lookup phase[0]: seconds: 19.144 | instructions: 2.4888e+10 | cycles: 9.65791e+10 | 
Insert phase[1]: seconds: 18.7149 | instructions: 3.59194e+10 | cycles: 9.44659e+10 | 
Lookup phase[1]: seconds: 19.2685 | instructions: 2.48929e+10 | cycles: 9.72303e+10 | 
Insert phase[2]: seconds: 19.2945 | instructions: 3.586e+10 | cycles: 9.73583e+10 | 
Lookup phase[2]: seconds: 19.8919 | instructions: 2.48823e+10 | cycles: 1.00377e+11 | 
Insert phase[3]: seconds: 18.2983 | instructions: 3.71527e+10 | cycles: 9.23313e+10 | 
Lookup phase[3]: seconds: 19.5427 | instructions: 2.53204e+10 | cycles: 9.84978e+10 | 
Insert phase[4]: seconds: 18.988 | instructions: 3.63872e+10 | cycles: 9.58645e+10 | 
Lookup phase[4]: seconds: 19.5874 | instructions: 2.53364e+10 | cycles: 9.88444e+10 | 


