// tc3 demonstrates:
// array

puts("Print Alphabet");
array a[26];
for (i = 0; i < 26; i = i + 1)
{
	a[i] = 'a' + i;
	putc_(a[i]);
	putc_(' ');
}