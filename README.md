<p align="center">
    <img src="https://github.com/yushou-cell/zsend/assets/59219725/d1b4c06e-2894-4989-8445-3537242b41c4" width="260" alt="示例图片">
</p>

# ZSEND - Efficient ZMQ Client

Currently, this project is used to send and construct zmq requests

|library name|Library version|license|
|---|---|---|
|STL|20(gcc-14)|GPLv3|
|glog|v0.7.0|BSD-3-Clause lisense|
|libczmq|v4.2.1|MPL-2.0 lisense|
|libzmq|v4.3.4|MPL-2.0 lisense|
|nlohmann::json|v3.11.3|MIT lisense|
|mimalloc|2.1.7|MIT lisense|

ZSEND is a versatile and efficient ZMQ Client designed to facilitate High concurrency, high traffic scenarios. It can be used both as a core component within the Zngin engine and independently deployed for managing request sending.

https://www.zxoud.com/zsend

## Features

- **Independent Deployment**: Designed for flexibility, zio can be deployed independently of the Zngin engine. This allows users to integrate and manage Routers without dependencies on other components.

- **Efficient Data Transfer**: Zio is developed using libzmq. It relies on libzmq to encapsulate the underlying IO threads, decouples the main routing operations from the distribution of requests and responses, greatly reducing the overhead of thread switching.

## Installation
To install ZSEND, follow these steps:

Clone the repository:
git clone https://github.com/ZXOUD/zio.git
## Contributing
We welcome contributions in all forms! If you have any suggestions, bug reports, or would like to contribute code, please feel free to submit an issue or pull request.

## License
This project is licensed under the MIT License.

## Contact
For issues, suggestions, and feedback, please open an issue on GitHub.
