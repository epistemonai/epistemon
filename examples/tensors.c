#include <epistemon/epistemon.h>
#include <epistemon/math/tensor.h>

static void init()
{
	float a0[3] = {1.0, 2.0, 3.0};
        float a1[3] = {4.0, 5.0, 6.0};

	epiTensor tensor1 = epi_tensor((epiTensorDesc){
		.shape={3},
		.set  =(float*)a0
	});

	epiTensor tensor2 = epi_tensor((epiTensorDesc){
		.shape={3},
		.set  =(float*)a1,
	});

        epi_tensor_transpose(&tensor1, &tensor1);
	//epi_tensor_dot(&tensor1, &tensor2, &tensor1);

	epi_tensor_print(&tensor1);

	epi_tensor_destroy(&tensor1);
	epi_tensor_destroy(&tensor2);
}

static void destroy()
{

}

static void tick()
{
	epi_running = false;
}

epiDesc epi_main()
{
	return (epiDesc){
		.init    = init,
		.destroy = destroy,
		.tick    = tick
	};
}
