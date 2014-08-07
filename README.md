#1. github上的操作

##1.1. 初始化操作
在http://github.com 上注册自己的github账户后，要执行语句
git config --global user.name "yourname"
git config --global user.email "XXX@XXX.com"
这样等于是在本地标记了自己的账户，以后就可以直接使用了。

##1.2. 在最后有获取密钥的方法

1. 要上传文件到GitHub的Git系统上，需要一个SSH密匙来认证，在文章最后有方法。
2. 将SSH Key提交到GitHub `在设置中添加`

##1.3. 基本git操作
上传代码到git的步骤：
1. git init
建立一个仓库。

2. git add XXX
添加文件XXX。add后面加“.”，添加当前目录所有文件。

3. git commit -m 'message' (`最后一个附加信息相当于标签`)
上传时附加说明信息message。

4. git remote add origin XXX@github.com:XXX/XXX.git   (`给git仓库的地址取一个别名`)
这一句将origin用作XXX@github.com:XXX/XXX.git的别名，以后就可以方便的用origin代表XXX@github.com:XXX/XXX.git了。
> git remote rm origin用于删除别名

5. git push -u origin master   上传带`master`标签的文件到github
上传当前目录到github的master分支

> 如果原来仓库中有另外一个文件, 首先应该进行github仓库代码下载, 合并后总体上传,因为github并不能识别代码是不是一个项目
