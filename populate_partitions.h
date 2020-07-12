struct cell_assignments{
	int total_partition_area_A;
	int total_partition_area_B;
	struct dll* list_of_cells_for_A;
	struct dll* list_of_cells_for_B;
};

void segregate_cells_randomly(struct cell**, int, struct partition*, struct partition*, double, int, int);
void segregate_cells_largest_cell_first();
void segregate_cells_through_genetic_algorithm();
void segregate_cells_with_neural_network();

void copy_cells_into_partitions(struct partition*, struct partition*, struct dll*, struct dll*, int, int);
