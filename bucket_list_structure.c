#include <stdio.h>
#include <stdlib.h>
#include "bucket_list_structure.h"
#include "dll_structure.h"

void initialize_bucket_list(struct bucket_list* partition){
	struct dll* gains = malloc(sizeof(gains));
	partition->gains = gains;

	partition->max_gain = NULL;
}
