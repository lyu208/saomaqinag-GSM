---
title: Markdown用法
date: 2018-04-11 19:11:51
tags: "Hexo"
categories: "学习"
---
<h3>Markdown用法</h3>
Hexo 是使用Markdown标记语言的，以前没有接触此处学习并记录。
<!-- more -->
## 什么是Markdown
Markdown具有一系列衍生版本，用于扩展Markdown的功能（如表格、脚注、内嵌HTML等等），这些功能原初的Markdown尚不具备，它们能让Markdown转换成更多的格式，例如LaTeX，Docbook。Markdown增强版中比较有名的有Markdown Extra、MultiMarkdown、 Maruku等。这些衍生版本要么基于工具，如Pandoc；要么基于网站，如GitHub和Wikipedia，在语法上基本兼容，但在一些语法和渲染效果上有改动。
## 标题
标题能显示出文章的结构。行首插入1-6个 # ，每增加一个 # 表示更深入层次的内容，对应到标题的深度由 1-6 阶。#号后面应使用空格与标题分隔
H1 :# 一级标题
H2 :## 二级标题
H3 :### 三级标题
H4 :#### 四级标题
H5 :##### 五级标题
H6 :###### 六级标题


## 文本样式
带星号的文本样式，在原版Markdown标准中不存在，但在其大部分衍生标准中被添加
链接 :[Title](https://www.github.com/chain01)
加粗 :**Bold**
斜体字 :*Italics*
删除线 :~~text~~
高亮 :==text==
段落 : 段落之间空一行
换行符 : 一行结束时输入两个空格
  
列表 :* 添加星号成为一个新的列表项。
引用 :
> 使用说明，要结束引用需要在引用部分结束后插入一个回车


内嵌代码 : `alert('Hello World');`
画水平线 (HR) :--------
列举
> * 整理知识，学习笔记
> * 发布日记，杂文，所见所想
> * 撰写发布技术文稿（代码支持）
> * 撰写发布学术论文（LaTeX 公式支持）

插入图片
![cmd-markdown-logo](https://www.zybuluo.com/static/img/logo.png)

- [ ] 不选中的框
- [x] 选中的框

## 书写一个质能守恒公式[^LaTeX]
$E=mc^2$

## 代码高亮
```python
@requires_authorization
class SomeClass:
    pass

if __name__ == '__main__':
    # A comment
    print 'hello world'
```

## 流程图
```flow
st=>start: Start
op=>operation: Your Operation
cond=>condition: Yes or No?
e=>end

st->op->cond
cond(yes)->e
cond(no)->op
```
## 绘制表格

| 左对齐       | 右对齐   |  居中  |
| --------   | -----:  | :----:  |
| 计算机     | $1600 |   5     |
| 手机        |   $12   |   12   |
| 管线        |    $1    |  234  |











