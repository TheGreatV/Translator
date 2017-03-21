`/ All of the following should parse correctly

.schema; `/ Unnamed stub schema
.schema 0; `/ Named stub schema

.schema {} `/ Unnamed full schema

.schema 1 { `/ Named full schema
	.schema; `/ Unnamed nested stub schema
	.schema 2; `/ Named nested stub schema
	.schema {} `/ Unnamed nested full schema
	.schema 3 {} `/ Named nested full schema
	
	.algorithm 0; `/ Stub bare algorithm
	.algorithm 1 .body; `/ Full bare algorithm with stub body
	.algorithm 2 .body {} `/ Full bare algorithm with full body
	
	.algorithm .schema;; `/ Algorithm with placement stub schema
	.algorithm .schema {}; `/ Algorithm with placement full schema
	
	.algorithm 3 .body {
		.schema; `/ Unnamed nested stub schema inside algorithm
		.schema 4; `/ Named nested stub schema inside algorithm
		.schema {} `/ Unnamed nested full schema inside algorithm
		.schema 5 {} `/ Named nested full schema inside algorithm
	}
}



