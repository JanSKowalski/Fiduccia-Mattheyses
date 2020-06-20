struct bucket_list{
	struct dll* gains;
	struct node* max_gain;
};

void initialize_bucket_list(struct bucket_list*);
