.block RawVariant {
	.schema Result {}
	.schema Argument {}

	.schema Schema {
		`/ Self-property
		.algorithm Result
		.body {} 
		
		`/ Self-functions
		.algorithm .return Result .accept (.make argument .of Argument)
		.body {}

		.algorithm .return Result .accept [.make argument .of Argument]
		.body {}
		
		.algorithm .return Result .accept {.make argument .of Argument}
		.body {}
		
		`/ Self-operators
		.algorithm .return Result
		.left .make argument .of Argument
		.body {}

		.algorithm .return Result
		.right .make argument .of Argument
		.body {}
		
		.algorithm .return Result
		.left .make argument .of Argument .right .make argument .of Argument
		.body {}
	}

	.make instance .of Schema;
	.make argument .of Argument;

	instance; `/ property
	instance(argument); `/ function ()
	instance[argument]; `/ function [] indexator
	instance{argument}; `/ function {}

	instance argument; `/ prefix
	argument instance; `/ postfix
	argument instance argument; `/ interfix
}

.block OptimizedVariant {
	.schema Result {}
	.schema Argument {}

	.schema Schema {
		`/ Self-property
		.algorithm Result
		.body {} 
		
		`/ Self-functions
		.algorithm Result (.make argument .of Argument)
		.body {}

		.algorithm Result [.make argument .of Argument]
		.body {}
		
		.algorithm Result {.make argument .of Argument}
		.body {}
		
		`/ Self-operators
		.algorithm Result .left .make argument .of Argument
		.body {}

		.algorithm Result .right .make argument .of Argument
		.body {}
		
		.algorithm Result .left .make argument .of Argument .right .make argument .of Argument
		.body {}
	}

	.make instance .of Schema;
	.make argument .of Argument;

	instance; `/ property
	instance(argument); `/ function ()
	instance[argument]; `/ function [] indexator
	instance{argument}; `/ function {}

	instance argument; `/ prefix
	argument instance; `/ postfix
	argument instance argument; `/ interfix
}

.block OptimizedVariant2 {
	.schema Result {}
	
	.make argument .of .schema Argument {}

	.make instance .of .schema Schema {
		`/ Self-property
		.algorithm Result .body {} 
		
		`/ Self-functions
		.algorithm Result (.make argument .of Argument) .body {}
		.algorithm Result [.make argument .of Argument] .body {}
		.algorithm Result {.make argument .of Argument} .body {}
		.algorithm Result (.make argument .of Argument} .body {}
		
		`/ Self-operators
		.algorithm Result .left .make argument .of Argument .body {}
		.algorithm Result .right .make argument .of Argument .body {}
		.algorithm Result .left .make argument .of Argument .right .make argument .of Argument .body {}
	}

	instance; `/ property
	instance(argument); `/ function ()
	instance[argument]; `/ function [] indexator
	instance{argument}; `/ function {}
	instance(argument}; `/ function {}

	instance argument; `/ prefix
	argument instance; `/ postfix
	argument instance argument; `/ interfix
}

.block FunctionDeclaration {

	.make Function .of .schema {
		.algorithm Result (.make argument .of Argument) .body {
		}
	}
	
	.alias Function(argument) .as result;
}

.block OperatorUsing {
	.make + .of .schema {
		.algorithm Result
			.left .make left .of Argument
			.right .make right .of Argument
		.body;
	}
	
	argument + argument;
}

























