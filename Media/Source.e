.schema;
.schema {}
.schema {
	.algorithm A;
	.algorithm .schema; .body;
	.algorithm .schema #; .body;
	.algorithm .schema;;
	.algorithm .schema{}; 
	.algorithm .schema{} .body;
	.algorithm .schema{
		.schema;
	} .body;
	.algorithm .schema {
		.schema { .algorithm .schema;; }
	} .body;
}
.schema { .schema A {} }
.schema A { .schema A { .schema A {} } }

`{
.schema A {
	.algorithm A .body {
		.make a .of A;
	}
}

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

