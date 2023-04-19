./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1 | diff - cases/task1/simple-sjf.out
echo ========================================================================
./allocate -f cases/task1/more-processes.txt -s SJF -m infinite -q 3 | diff - cases/task1/more-processes.out
echo ========================================================================
./allocate -f cases/task2/simple.txt -s RR -m infinite -q 3 | diff - cases/task2/simple-rr.out
echo ========================================================================
./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 1 | diff - cases/task2/two-processes-1.out
echo ========================================================================
./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 3 | diff - cases/task2/two-processes-3.out
echo ========================================================================
./allocate -f cases/task3/simple.txt -s SJF -m best-fit -q 3 | diff - cases/task3/simple-bestfit.out
echo ========================================================================
./allocate -f cases/task3/non-fit.txt -s SJF -m best-fit -q 3 | diff - cases/task3/non-fit-sjf.out
echo ========================================================================
./allocate -f cases/task3/non-fit.txt -s RR -m best-fit -q 3 | diff - cases/task3/non-fit-rr.out
echo ========================================================================
./allocate -f cases/task4/spec.txt -s SJF -m infinite -q 3 | diff - cases/task4/spec.out
echo ========================================================================
./allocate -f cases/task1/more-processes.txt -s SJF -m infinite -q 3 | diff - cases/task1/more-processes.out
echo ========================================================================
./allocate -f cases/task2/simple.txt -s RR -m infinite -q 3 | diff - cases/task2/simple-rr.out
echo ========================================================================
./allocate -f cases/task1/simple.txt -s SJF -m infinite -q 1 | diff - cases/task1/simple-sjf.out
echo ========================================================================
./allocate -f cases/task2/two-processes.txt -s RR -m infinite -q 3 | diff - cases/task2/two-processes-3.out