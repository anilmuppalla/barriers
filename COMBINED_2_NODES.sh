for i in 2 3 4 5 6 7 8 9 10 11 12
do
	#PBS -q cs6210 #PBS -l nodes=2:sixcore #PBS -l walltime=00:05:00 #PBS -N COMBINED_2_NODES /opt/openmpi-1.4.3-gcc44/bin/mpirun --hostfile $PBS_NODEFILE -np 2 combined.out $i 100
done
