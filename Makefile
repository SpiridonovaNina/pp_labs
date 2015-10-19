all:
	g++ pp_1.cpp -o pp_1 -lpthread
	g++ pp_2.cpp -o pp_2 -lpthread
	g++ pp_3.cpp -o pp_3 -lpthread
	g++ pp_4.cpp -o pp_4 -lpthread
	g++ pp_5.cpp -o pp_5 -lpthread
	g++ pp_6.cpp -o pp_6 -lpthread
	g++ pp_7.cpp -o pp_7 -lpthread
	g++ pp_8.cpp -o pp_8 -lpthread
	mpicxx -o pp_91 pp_91.cpp
	mpicxx -o pp_92 pp_92.cpp
	mpicxx -o pp_93 pp_93.cpp