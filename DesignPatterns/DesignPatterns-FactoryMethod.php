
//相同业务数据 的 不同输出或反馈体现

<?php

namespace RefactoringGuru\FactoryMethod\Conceptual;

//设计一个连接服务器的框架，需要三个协议，"POP3"、"IMAP"、"HTTP"，可以把这三个作为产品类，共同实现一个接口

abstract class Creator //Factory
{

    abstract public function factoryMethod(): Product;


    public function someOperation(): string
    {
        // Call the factory method to create a Product object.
        $product = $this->factoryMethod();
        // Now, use the product.
        $result = "Creator: " .
            $product->operation();

        return $result;
    }
}


class ConcreteCreator1 extends Creator
{

    public function factoryMethod(): Product
    {
        return new httpserver();
    }
}

class ConcreteCreator2 extends Creator
{
    public function factoryMethod(): Product
    {
        return new imapserver();
    }
}


interface Product
{
    public function operation(): string;
}


class httpserver implements Product
{
	public function __construct()
    {
        echo "init httpserver\n";
    }
	
    public function operation(): string
    {
        return "conn httpserver\n";
    }
}

class imapserver implements Product
{
	public function __construct()
    {
        echo "init imapserver\n";
    }
	
    public function operation(): string
    {
        return "conn imapserver\n";
    }
}


function clientCode(Creator $creator)
{
    // ...
    echo "Client: \n"
        . $creator->someOperation();
    // ...
}


echo "App: Launched with the ConcreteCreator1.\n";
clientCode(new ConcreteCreator1());
echo "\n\n";

echo "App: Launched with the ConcreteCreator2.\n";
clientCode(new ConcreteCreator2());

?>



<?php


namespace RefactoringGuru\FactoryMethod\Conceptual;

//设计一个连接服务器的框架，需要三个协议，"POP3"、"IMAP"、"HTTP"，可以把这三个作为产品类，共同实现一个接口

class Factory
{
	public function factoryMethod($type): Product
    {
		if($type == null){
			 return null;
		  }        
		  if($type == "http"){
			 return new httpserver();
		  } else if($type == "imap"){
			 return new imapserver();
		  }
		  
		  return null;
    }
}

interface Product
{
    public function operation(): string;
}


class httpserver implements Product
{
	public function __construct()
    {
        echo "init httpserver\n";
    }
	
    public function operation(): string
    {
        return "conn httpserver\n";
    }
}

class imapserver implements Product
{
	public function __construct()
    {
        echo "init imapserver\n";
    }
	
    public function operation(): string
    {
        return "conn imapserver\n";
    }
}

$serverFactory = new Factory();

$xx = $serverFactory->factoryMethod("http");
echo $xx->operation();


$xx = $serverFactory->factoryMethod("imap");
echo $xx->operation();


/*
output:

init httpserver
conn httpserver
init imapserver
conn imapserver


*/

?>


