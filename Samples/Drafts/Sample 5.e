`/ features with coma "," statement

.block Make {
	`/ multiple instance definition
	.make a1, a2 .of A;

	`/ multiple instance of different schemas definition
	.make
		a .of A,
		b .of B;

	`/ multiple instances multiple schemas
	.make
		b1, b2 .of B,
		c1, c2, c3 .of C;
}

.block Schema {
	`/ multiple schema definition
	.schema
		A {},
		B {};
}

.block Alias {
	`/ multiple alias
	.alias
		a .as b,
		c .as d;
}

