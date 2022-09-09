pip3 install torch==1.12.1+cu102 torchvision==0.13.1+cu102 torchaudio==0.12.1 --extra-index-url https://download.pytorch.org/whl/cu102

export TORCH_INSTALL=https://developer.download.nvidia.cn/compute/redist/jp/v50/pytorch/torch-1.11.0a0+bfe5ad28.nv22.1-cp36-cp36-linux_aarch64.whl

python3 -m pip install --upgrade pip; python3 -m pip install expecttest xmlrunner hypothesis aiohttp numpy=='1.19.4' pyyaml scipy=='1.5.3' ninja cython typing_extensions protobuf; export "LD_LIBRARY_PATH=/usr/lib/llvm-8/lib:$LD_LIBRARY_PATH"; python3 -m pip install --upgrade protobuf; python3 -m pip install --no-cache $TORCH_INSTALL
