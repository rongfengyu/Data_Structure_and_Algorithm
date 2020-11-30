<?php

/*
//һ���Ʒ�Ĳ�ͬά�Ƚ���

���磺

	�Բ�ͬ�Ĳ�Ʒ�������Ʒ����Ϊά�Ȼ��ƾ���

	X ���������  http��imap ����
	Y ����ϵͳ	windows��linux ����
	Z Ӳ��ƽ̨	X86��mips����
	
	�����໥��ϣ���ᴴ��X*Y*Z�ֲ�ͬ�Ķ���

*/


namespace RefactoringGuru\FactoryMethod\Conceptual;

//���һ�����ӷ������Ŀ�ܣ���Ҫ����Э�飬"POP3"��"IMAP"��"HTTP"�����԰���������Ϊ��Ʒ�࣬��ͬʵ��һ���ӿڣ��ܹ����ݲ�ͬ����ϵͳƽ̨windows��linux


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
//һ���Ʒ�Ĳ�ͬά�Ƚ���

���磺

	�Բ�ͬ�Ĳ�Ʒ�������Ʒ����Ϊά�Ȼ��ƾ���

	X ���������  http��imap ����
	Y ����ϵͳ	windows��linux ����
	Z Ӳ��ƽ̨	X86��mips����
	
	�����໥��ϣ���ᴴ��X*Y*Z�ֲ�ͬ�Ķ���

*/


namespace RefactoringGuru\FactoryMethod\Conceptual;

//���һ�����ӷ������Ŀ�ܣ���Ҫ����Э�飬"POP3"��"IMAP"��"HTTP"�����԰���������Ϊ��Ʒ�࣬��ͬʵ��һ���ӿڣ��ܹ����ݲ�ͬ����ϵͳƽ̨windows��linux

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