# httpd

- [httpd](#httpd)
  - [概述](#概述)
  - [编译](#编译)
  - [规范](#规范)
    - [命名规范](#命名规范)
  - [参考](#参考)

## 概述

造轮子，实现一个http的服务器。

## 编译

```
$ make

$ ./bin/httpd
```

> Ubuntu中需要将`-lpthread`放在最后

## 规范

### 命名规范

1. 变量，函数名和命名空间使用sanke_case(蛇形命名法)，全局变量使用`g_`作为前缀
2. 自定义类名用CamelCase(大驼峰命名法)
3. 成员函数用sanke_case
4. 成员变量用sanke_case, 使用`m_`作为前缀
5. 宏和常量全大写，用下滑线连接
6. 下划线开头明明只用在`#ifndef...`中

## 参考

[《UNIX网络编程 卷1：套接字联网API》第3版 人民邮电出版社](https://book.douban.com/subject/1500149/)
