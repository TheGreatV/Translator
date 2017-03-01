.schema A {
	.algorithm A .body {
		.make a .of A;
	}
}
`{
.make A + -.make B;

Parse:

1. extract units
%0 = .make A
%1 = +
%2 = -
%3 = .make B

%0 %1 %2 %3

2. pick by priority

}`

