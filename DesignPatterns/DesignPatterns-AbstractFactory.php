<?php

/*
//一款产品的不同维度解析

例如：

	以不同的产品类型与产品变体为维度绘制矩阵。

	X 服务器软件  http，imap ……
	Y 操作系统	windows、linux ……
	Z 硬件平台	X86、mips……
	
	他们相互耦合，则会创造X*Y*Z种不同的对象

*/


namespace RefactoringGuru\FactoryMethod\Conceptual;

//设计一个连接服务器的框架，需要三个协议，"POP3"、"IMAP"、"HTTP"，可以把这三个作为产品类，共同实现一个接口，能够兼容不同操作系统平台windows、linux


interface AbstractFactory
{
    public function createHttp(): httpserver;

    public function createImap(): imapserver;
}

class ConcreteWindows implements  AbstractFactory
{

    public function createHttp(): httpserver
    {
        return new httpserverWindows();
    }
	
	public function createImap(): imapserver
	{
		return new imapserverWindows();
	}
}

class ConcreteLinux implements  AbstractFactory
{

    public function createHttp(): httpserver
    {
        return new httpserverLinux();
    }
	
	public function createImap(): imapserver
	{
		return new imapserverLinux();
	}
}

interface httpserver
{
    public function httpserverinfo(): string;
}

class httpserverWindows implements  httpserver
{

    public function httpserverinfo(): string
    {
        return "conn httpserverWindows";
    }
}

class httpserverLinux implements  httpserver
{

    public function httpserverinfo(): string
    {
        return "conn httpserverLinux";
    }
}

interface imapserver
{
    public function imapserverinfo(): string;
}

class imapserverWindows implements  imapserver
{

    public function imapserverinfo(): string
    {
        return "conn imapserverWindows";
    }
}

class imapserverLinux implements  imapserver
{

    public function imapserverinfo(): string
    {
        return "conn imapserverLinux";
    }
}

function clientCode(AbstractFactory $factory)
{
    $productA = $factory->createHttp();
    $productB = $factory->createImap();

	echo $productA->httpserverinfo() . "\n";
    echo $productB->imapserverinfo() . "\n";
    //echo $productB->anotherUsefulFunctionB($productA) . "\n";
}


echo "Client: Testing client code with the first factory type:\n";
clientCode(new ConcreteWindows());


echo "\n";

echo "Client: Testing the same client code with the second factory type:\n";
clientCode(new ConcreteLinux());
?>



<?php

/*
//一款产品的不同维度解析

例如：

	以不同的产品类型与产品变体为维度绘制矩阵。

	X 服务器软件  http，imap ……
	Y 操作系统	windows、linux ……
	Z 硬件平台	X86、mips……
	
	他们相互耦合，则会创造X*Y*Z种不同的对象

*/


namespace RefactoringGuru\FactoryMethod\Conceptual;

//设计一个连接服务器的框架，需要三个协议，"POP3"、"IMAP"、"HTTP"，可以把这三个作为产品类，共同实现一个接口，能够兼容不同操作系统平台windows、linux

class AbstractFactory
{
	public function factoryMethod($type): AbstractFactory
    {
		if($type == null){
			 return null;
		  }        
		  if($type == "Windows"){
			 return new ConcreteWindows();
		  } else if($type == "Linux"){
			 return new ConcreteLinux();
		  }
		  
		  return null;
    }
}

class ConcreteWindows extends  AbstractFactory
{

    public function createHttp(): httpserver
    {
        return new httpserverWindows();
    }
	
	public function createImap(): imapserver
	{
		return new imapserverWindows();
	}
}

class ConcreteLinux extends  AbstractFactory
{

    public function createHttp(): httpserver
    {
        return new httpserverLinux();
    }
	
	public function createImap(): imapserver
	{
		return new imapserverLinux();
	}
}

interface httpserver
{
    public function httpserverinfo(): string;
}

class httpserverWindows implements  httpserver
{

    public function httpserverinfo(): string
    {
        return "conn httpserverWindows";
    }
}

class httpserverLinux implements  httpserver
{

    public function httpserverinfo(): string
    {
        return "conn httpserverLinux";
    }
}

interface imapserver
{
    public function imapserverinfo(): string;
}

class imapserverWindows implements  imapserver
{

    public function imapserverinfo(): string
    {
        return "conn imapserverWindows";
    }
}

class imapserverLinux implements  imapserver
{

    public function imapserverinfo(): string
    {
        return "conn imapserverLinux";
    }
}

function clientCode(AbstractFactory $factory)
{
    $productA = $factory->createHttp();
    $productB = $factory->createImap();

	echo $productA->httpserverinfo() . "\n";
    echo $productB->imapserverinfo() . "\n";
    //echo $productB->anotherUsefulFunctionB($productA) . "\n";
}


$serverFactory = new AbstractFactory();

echo "Client: Testing client code with the first factory type:\n";
$xx = $serverFactory->factoryMethod("Windows");
clientCode($xx);


echo "\n";

echo "Client: Testing the same client code with the second factory type:\n";
$xx = $serverFactory->factoryMethod("Linux");
clientCode($xx);


/*

Client: Testing client code with the first factory type:
conn httpserverWindows
conn imapserverWindows
Client: Testing the same client code with the second factory type:
conn httpserverLinux
conn imapserverLinux

*/


?>