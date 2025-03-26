
from skbuild import setup

setup(
    name="bite",
    version="0.3.0",
    author="David Schaurecker",
    author_email="david.schaurecker@gmail.com",
    description="A Python wrapper for my C++ battery CID arbitrage simulation engine.",
    long_description="...",
    python_requires=">=3.8",
    install_requires=[
        'pybind11>=2.5.0',
        'numpy>=1.16.0',
        'pandas>=0.24.0',
        'matplotlib>=3.0.0',
        'tqdm>=4.0.0',
        # ... other Python deps
    ],
    zip_safe=False,
    include_package_data=True,
    packages=["bite"],
    package_data={"bite": ["bins/*"]},
)