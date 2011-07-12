#include <stdio.h>

#define BUFSIZE	1024

unsigned long fetch_lun(FILE *fp, const char *devname, int lun)
{
	int	ln;
	int	mln;
	char	bus[40];
	int	mtno;
	char	module_type[40];
	int	lu;
	char	W1[40];
	char	AM1[40];
	char	DPsz1[40];
	char	basaddr1[40];
	char	range1[40];
	char	W2[40];
	char	AM2[40];
	char	DPsz2[40];
	char	basaddr2[40];
	char	range2[40];
	char	testoff[40];
	int	sz;
	int	sl;
	int	ss;
	int	L;
	int	Vect;

	char	buf[BUFSIZE];

	while (fgets(buf, BUFSIZE, fp) != NULL) {
		fscanf(fp, "#+# %d%d%s%d%s%d%s%s%s%s%s%s%s%s%s%s%s%s%s%d%d%d%d%d",
			&ln, &mln, &bus, &mtno, &module_type, &lu, &W1, &AM1,
			&DPsz1, &basaddr1, &range1, &W2, &AM2, &DPsz2, &basaddr2,
			&range2, &testoff, &sz, &sl, &ss, &L,
			&Vect);
		if (strcmp(devname, module_type) == 0 && lu == lun)
			return strtoul(basaddr1, NULL, 16);
	}
	return 0;
}

int main(int argc, char *argv[])
{
	FILE *f;

	f = fopen(argv[1], "r");
	printf("0x%08x\n", fetch_lun(f, "CVORA-TMP", 0));
}
