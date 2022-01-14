# httpd

- [httpd](#httpd)
  - [概述](#概述)
  - [快速开始](#快速开始)
  - [规范](#规范)
    - [命名规范](#命名规范)

## 概述

`httpd`是使用C/C++开发的Linux上的httpd服务器程序。

实现版本：

- http 1.1

## 快速开始

```
$ make

$ ./bin/httpd
```

> Ubuntu系统中需要将`-lpthread`放在最后

## 规范

### 命名规范

1. 变量，函数名和命名空间使用sanke_case(蛇形命名法)，全局变量使用`g_`作为前缀
2. 自定义类名用CamelCase(大驼峰命名法)
3. 成员函数用sanke_case
4. 成员变量用sanke_case, 使用`m_`作为前缀
5. 宏和常量全大写，用下滑线连接
6. 下划线开头明明只用在`#ifndef...`中
