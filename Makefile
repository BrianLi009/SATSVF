SRC = generate

all: compile dump png

init:
	# If use docker, I guess no need for add bin to path.
	# ae  cfl  dvf  llvm2svf  mta  saber  svf-ex  wpa
	export PATH=$PATH:/mnt/c/Users/creat/Desktop/svf-proj/node_modules/SVF/Release-build/bin
compile:
	clang++ -c -emit-llvm $(SRC).cpp -o $(SRC)_ori.bc
	# 1. use clang to compile it
	# 2. execute `llvm-nm $(SRC)_ori.bc` to find important function.
	# 3. extract only useful function.
	llvm-extract -func=main -func=_Z4combii \
		-func=_Z19vertex_combinationsRSt6vectorIS_IiSaIiEESaIS1_EEii \
		-func=_Z17edge_combinationsRKSt6vectorIiSaIiEE \
		-func=_Z8generateiii \
		-o $(SRC).bc $(SRC)_ori.bc
dump:
	# wpa -ander $(SRC).bc
	# wpa -ander -dump-callgraph $(SRC).bc 
	# wpa -fspta -dump-callgraph $(SRC).bc 
	# wpa -ander -dump-pag $(SRC).bc 
	# wpa -ander -dump-constraint-graph $(SRC).bc 
	wpa -ander -dump-icfg $(SRC).bc  -extapi=false -svfg-with-ind-call=false
	# wpa -ander -svfg -dump-vfg $(SRC).bc 
	# wpa -ander -svfg -dump-mssa $(SRC).bc 
png:
	# dot -Tpng callgraph_final.dot -o callgraph_final.png
	# dot -Tpng callgraph_initial.dot -o callgraph_initial.png
	# dot -Tpng consCG_final.dot -o consCG_final.png
	dot -Tpng icfg_initial.dot -o icfg_initial.png
	# dot -Tpng consCG_initial.dot -o consCG_initial.png
	# dot -Tpng svfg_final.dot -o svfg_final.png
	# dot -Tpng svfir_initial.dot -o svfir_initial.png

