CC=clang

danhSach = chamMuc.o PNG.o

chamMuc: $(danhSach)
	$(CC) -o chamMuc -lz -lm $(danhSach)

# ==== CHẤM MỰC
chamMuc.o: chamMuc.c
	$(CC) -c chamMuc.c

# ==== TẬP TIN ẢNH
PNG.o: PNG.c PNG.h
	$(CC) -c PNG.c


# ==== LÀM SẠCH
clean:	
	rm $(danhSach)
