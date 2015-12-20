// tc2 demonstrates:
// function

fibonacci(n)
{
	if ((n < 0) || (n > 40))
	{
		puts("Invalid number");
		return -1;
	}
	else if (n == 0)
	{
		return 0;
	}
	else if (n == 1)
	{
		return 1;
	}
	else {
		return fibonacci(n - 1) + fibonacci(n - 2);
	}
}

puts_("Input the term you want to calculate: ");
result = fibonacci(n);
if (result >= 0)
	puti(result);