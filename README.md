# CJson
想要用C/C++语言，配合STL库，运用有限状态自动机理论和LL(1)文法，来实现简单的Json格式的解析器。
大概的流程：
1. 解决string的解析问题 （基本解决，换行符等无法实现，unicode只能解析英文部分）
2. 解决number的解析问题 
3. 解决whitespace的解析问题（基本解决）
4. 解决value的解析问题 
5. 解决array的解析问题 
6. 解决object的解析问题 
7. 解决json的输出问题 

注意：如果想要完成中文的解析，请将txt修改为ANSI编码。如果是UTF-8编码会无法正常识别中文字符。

![Alt text](./pic/encode.png)

例如：number的词法分析，BNF范式给出如下：

$$
\begin{align}
& number::= ['-']\ int\ [ frac ]\ [ exp ]\\
& int::= '0'\ |\ ([1-9][0-9]^{*}) \\
& exp::= ('e'|'E')\ ['-' | '+']\ [0-9]^{+}
\end{align} 
$$

参考链接：http://www.json.org/json-zh.html

想要完成的目标：
* 完成Json的识别和存储
* 实现Json格式的美化输出
* 完成多格式的转换，如Json和XML的转换
* 输出潜在的错误位置
* 随机Json生成器
* ...