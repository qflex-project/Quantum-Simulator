for q in 25 27 29 31
do
	echo "Processing qubits $q"
	for e in 4
	do
		for ((l = q - 7; l <= q - 3; l++))
		do
			for c in 16
			do
				./shor.out $q $e $l $c
			done
		done
	done
done