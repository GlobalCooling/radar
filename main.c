#include "sys.h"
#include "delay.h"

//串口1句柄
UART_HandleTypeDef UART1_Handler;
//初始化串口,bound：波特率
void UART1_Init(u32 bound){
	UART1_Handler.Instance=USART1;//串口1
	UART1_Handler.Init.BaudRate=bound;//波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;//字长
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;//不开启硬件流控制
	UART1_Handler.Init.Mode=UART_MODE_TX;//仅发送
	UART1_Handler.Init.Parity=UART_PARITY_NONE;//无校验位
	HAL_UART_Init(&UART1_Handler);
}

//配置串口IO及复用设置
void HAL_UART_MspInit(UART_HandleTypeDef *huart){
	GPIO_InitTypeDef GPIO_Initure;
	if(huart->Instance==USART1){//判断串口1
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);		//初始化PA9
	}
}

//--------------------------------------------------------------------------------------------
//初始化LED
void LED_Init(void){
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();//使能GPIOB时钟
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;//PB0和PB1分别是绿灯和红灯
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);//PB0置0，绿灯常亮
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);//PB1置1，红灯灭
}

//--------------------------------------------------------------------------------------------
//初始化蜂鸣器
void BEEP_Init(void){
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOC_CLK_ENABLE();//使能GPIOC时钟
	GPIO_Initure.Pin=GPIO_PIN_1;//PC1
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;//高速
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);//PC1置1，蜂鸣器关闭
}

//--------------------------------------------------------------------------------------------
//超声波模块
void Ultrasonic_Wave(){
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();//使能GPIOB时钟
	GPIO_Initure.Pin=GPIO_PIN_10;//PB10
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;//上拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);//关闭超声波启动高电平

	GPIO_Initure.Pin=GPIO_PIN_12;//PB12
	GPIO_Initure.Mode=GPIO_MODE_INPUT;//输入
	GPIO_Initure.Pull=GPIO_PULLDOWN;//下拉
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//高速
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

//--------------------------------------------------------------------------------------------
//定时器3输出PWM
TIM_HandleTypeDef TIM3_PWM_Handler;//定时器3句柄
TIM_OC_InitTypeDef TIM3_OC_Init;//定时器3通道1句柄
void TIM3_PWM_Init(){
	TIM3_PWM_Handler.Instance=TIM3;
	TIM3_PWM_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数
	TIM3_PWM_Handler.Init.Prescaler=900-1;//分频
	TIM3_PWM_Handler.Init.Period=2000-1;//自动装载值，50Hz，20ms
	HAL_TIM_PWM_Init(&TIM3_PWM_Handler); //初始化PWM
	
	//通道配置
	TIM3_OC_Init.OCMode=TIM_OCMODE_PWM1;//模式1
	TIM3_OC_Init.Pulse=50;//比较值，用来设置占空比
	TIM3_OC_Init.OCPolarity=TIM_OCPOLARITY_HIGH;//输出极性高，TIMx_CNT<TIMx_CCR输出为高电平 
	//配置TIM3通道1
	HAL_TIM_PWM_ConfigChannel(&TIM3_PWM_Handler,&TIM3_OC_Init,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM3_PWM_Handler,TIM_CHANNEL_1);//开启PWM通道1
}
//定时器3时钟使能，引脚配置
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim){
	__HAL_RCC_TIM3_CLK_ENABLE();//使能定时器3时钟
	__HAL_RCC_GPIOA_CLK_ENABLE();//开启GPIOA时钟
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Pin=GPIO_PIN_6;//PA6
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;//复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;//上拉
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;//高速
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;//复用为TIM3 CH1
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);//初始化PA6
}

//--------------------------------------------------------------------------------------------
//定时器4计数
TIM_HandleTypeDef TIM4_Handler;//定时器4句柄 
void TIM4_Init(){
	TIM4_Handler.Instance=TIM4;//通用定时器4
	TIM4_Handler.Init.Prescaler=900-1;//分频系数
	TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数
	TIM4_Handler.Init.Period=50000-1;//自动装载值 500ms，稍微设长一点，以免计数不够用
	TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
	HAL_TIM_Base_Init(&TIM4_Handler);
	//HAL_TIM_Base_Start(&TIM4_Handler);//使能定时器4
}
//开启定时器4时钟，此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim){
	__HAL_RCC_TIM4_CLK_ENABLE();//使能TIM4时钟
}

int main(void){		
	HAL_Init();//初始化HAL库和时钟,180MHz
	Stm32_Clock_Init(360,25,2,8);
	//初始化延时函数
	delay_init(180);
	
	//初始化led，蜂鸣器，超声波模块
	LED_Init();
	BEEP_Init();
	Ultrasonic_Wave();
	
	u8 rotation=1;//舵机旋转方向，1：正转，0：反转
	u8 pwm=50;//设置占空比，50->250(对应占空比：2.5%->12.5%，对应角度：0°->180°)
	TIM3_PWM_Init();//启动PWM输出
	
	TIM4_Init();//初始化TIM4计数

	u32 count=0;//用于保存TIM4的计数值

	u8 MAX_TIME=2;//等待超声波返回的最大时间（毫秒），决定整个系统的最远探测距离，注意超声波往返的时间计算

	//初始化串口和发送数据的数组
	UART1_Init(76800);
	u8 buffer[3]={0,0,'\n'};//数组三个元素分别存放定时器4计数值，当前占空比值，换行符

	//主循环：启动超声波模块->等待高电平->定时器4开始计数->高电平结束或者计数到达设定值->保存数据，关闭定时器->串口发送数据->根据情况操作led和蜂鸣器->修改占空比
	while(1){	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);//持续15us高电平启动超声波模块
		delay_us(15);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);//关闭超声波高电平
	
		while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0){//等待超声波模块echo脚的高电平
		}
		HAL_TIM_Base_Start(&TIM4_Handler);//开启定时器4，开始计数

		while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1){//等待超声波模块echo脚高电平结束
			if(TIM4->CNT>MAX_TIME*100){//超时提前退出，*100是根据定时器分频计算的
				break;
			}
		}
		count=TIM4->CNT;//先保存定时器的计数值
		HAL_TIM_Base_Stop(&TIM4_Handler);//关闭定时器4
		TIM4->CNT=0;//清空定时器4计数

		if(count>=MAX_TIME*100){//如果超过最大距离
			buffer[0]=MAX_TIME*100;
		}
		buffer[0]=count&0xFF;//低八位
		buffer[1]=pwm;

		HAL_UART_Transmit(&UART1_Handler,buffer,sizeof(buffer),1000);//串口发送数据
		//delay_ms(500);

		//根据情况操作led和蜂鸣器
		if (count<=MAX_TIME*50){//如果在报警距离内
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);//PB1置0，红灯亮
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);//PC1置0，蜂鸣器启动
		}
		
		if(rotation){//判断旋转方向以修改占空比
			pwm+=1;
			TIM3->CCR1=pwm;//修改占空比
			delay_ms(30);
		}else{
			pwm-=1;
			TIM3->CCR1=pwm;//修改占空比
			delay_ms(30);
		}

		if(pwm==250){
			rotation=0;//pwm增至250后，方向为反转
		}			
		if(pwm==50){
			rotation=1;//pwm减至50后，方向改为正转
		}

		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);//PB1置1，红灯灭
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);//PC1置1，蜂鸣器关闭
	}
}
 

