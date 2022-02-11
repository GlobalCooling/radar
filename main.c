#include "sys.h"
#include "delay.h"

//����1���
UART_HandleTypeDef UART1_Handler;
//��ʼ������,bound��������
void UART1_Init(u32 bound){
	UART1_Handler.Instance=USART1;//����1
	UART1_Handler.Init.BaudRate=bound;//������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;//�ֳ�
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;//������Ӳ��������
	UART1_Handler.Init.Mode=UART_MODE_TX;//������
	UART1_Handler.Init.Parity=UART_PARITY_NONE;//��У��λ
	HAL_UART_Init(&UART1_Handler);
}

//���ô���IO����������
void HAL_UART_MspInit(UART_HandleTypeDef *huart){
	GPIO_InitTypeDef GPIO_Initure;
	if(huart->Instance==USART1){//�жϴ���1
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);		//��ʼ��PA9
	}
}

//--------------------------------------------------------------------------------------------
//��ʼ��LED
void LED_Init(void){
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();//ʹ��GPIOBʱ��
	GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;//PB0��PB1�ֱ����̵ƺͺ��
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//�������
	GPIO_Initure.Pull=GPIO_PULLUP;//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET);//PB0��0���̵Ƴ���
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);//PB1��1�������
}

//--------------------------------------------------------------------------------------------
//��ʼ��������
void BEEP_Init(void){
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOC_CLK_ENABLE();//ʹ��GPIOCʱ��
	GPIO_Initure.Pin=GPIO_PIN_1;//PC1
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//�������
	GPIO_Initure.Pull=GPIO_PULLUP;//����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;//����
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);//PC1��1���������ر�
}

//--------------------------------------------------------------------------------------------
//������ģ��
void Ultrasonic_Wave(){
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_GPIOB_CLK_ENABLE();//ʹ��GPIOBʱ��
	GPIO_Initure.Pin=GPIO_PIN_10;//PB10
	GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;//�������
	GPIO_Initure.Pull=GPIO_PULLUP;//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);//�رճ����������ߵ�ƽ

	GPIO_Initure.Pin=GPIO_PIN_12;//PB12
	GPIO_Initure.Mode=GPIO_MODE_INPUT;//����
	GPIO_Initure.Pull=GPIO_PULLDOWN;//����
	GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;//����
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}

//--------------------------------------------------------------------------------------------
//��ʱ��3���PWM
TIM_HandleTypeDef TIM3_PWM_Handler;//��ʱ��3���
TIM_OC_InitTypeDef TIM3_OC_Init;//��ʱ��3ͨ��1���
void TIM3_PWM_Init(){
	TIM3_PWM_Handler.Instance=TIM3;
	TIM3_PWM_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���
	TIM3_PWM_Handler.Init.Prescaler=900-1;//��Ƶ
	TIM3_PWM_Handler.Init.Period=2000-1;//�Զ�װ��ֵ��50Hz��20ms
	HAL_TIM_PWM_Init(&TIM3_PWM_Handler); //��ʼ��PWM
	
	//ͨ������
	TIM3_OC_Init.OCMode=TIM_OCMODE_PWM1;//ģʽ1
	TIM3_OC_Init.Pulse=50;//�Ƚ�ֵ����������ռ�ձ�
	TIM3_OC_Init.OCPolarity=TIM_OCPOLARITY_HIGH;//������Ըߣ�TIMx_CNT<TIMx_CCR���Ϊ�ߵ�ƽ 
	//����TIM3ͨ��1
	HAL_TIM_PWM_ConfigChannel(&TIM3_PWM_Handler,&TIM3_OC_Init,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM3_PWM_Handler,TIM_CHANNEL_1);//����PWMͨ��1
}
//��ʱ��3ʱ��ʹ�ܣ���������
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim){
	__HAL_RCC_TIM3_CLK_ENABLE();//ʹ�ܶ�ʱ��3ʱ��
	__HAL_RCC_GPIOA_CLK_ENABLE();//����GPIOAʱ��
	GPIO_InitTypeDef GPIO_Initure;
	GPIO_Initure.Pin=GPIO_PIN_6;//PA6
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;//����
	GPIO_Initure.Speed=GPIO_SPEED_HIGH;//����
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;//����ΪTIM3 CH1
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);//��ʼ��PA6
}

//--------------------------------------------------------------------------------------------
//��ʱ��4����
TIM_HandleTypeDef TIM4_Handler;//��ʱ��4��� 
void TIM4_Init(){
	TIM4_Handler.Instance=TIM4;//ͨ�ö�ʱ��4
	TIM4_Handler.Init.Prescaler=900-1;//��Ƶϵ��
	TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���
	TIM4_Handler.Init.Period=50000-1;//�Զ�װ��ֵ 500ms����΢�賤һ�㣬�������������
	TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//ʱ�ӷ�Ƶ����
	HAL_TIM_Base_Init(&TIM4_Handler);
	//HAL_TIM_Base_Start(&TIM4_Handler);//ʹ�ܶ�ʱ��4
}
//������ʱ��4ʱ�ӣ��˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim){
	__HAL_RCC_TIM4_CLK_ENABLE();//ʹ��TIM4ʱ��
}

int main(void){		
	HAL_Init();//��ʼ��HAL���ʱ��,180MHz
	Stm32_Clock_Init(360,25,2,8);
	//��ʼ����ʱ����
	delay_init(180);
	
	//��ʼ��led����������������ģ��
	LED_Init();
	BEEP_Init();
	Ultrasonic_Wave();
	
	u8 rotation=1;//�����ת����1����ת��0����ת
	u8 pwm=50;//����ռ�ձȣ�50->250(��Ӧռ�ձȣ�2.5%->12.5%����Ӧ�Ƕȣ�0��->180��)
	TIM3_PWM_Init();//����PWM���
	
	TIM4_Init();//��ʼ��TIM4����

	u32 count=0;//���ڱ���TIM4�ļ���ֵ

	u8 MAX_TIME=2;//�ȴ����������ص����ʱ�䣨���룩����������ϵͳ����Զ̽����룬ע�ⳬ����������ʱ�����

	//��ʼ�����ںͷ������ݵ�����
	UART1_Init(76800);
	u8 buffer[3]={0,0,'\n'};//��������Ԫ�طֱ��Ŷ�ʱ��4����ֵ����ǰռ�ձ�ֵ�����з�

	//��ѭ��������������ģ��->�ȴ��ߵ�ƽ->��ʱ��4��ʼ����->�ߵ�ƽ�������߼��������趨ֵ->�������ݣ��رն�ʱ��->���ڷ�������->�����������led�ͷ�����->�޸�ռ�ձ�
	while(1){	
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);//����15us�ߵ�ƽ����������ģ��
		delay_us(15);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);//�رճ������ߵ�ƽ
	
		while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==0){//�ȴ�������ģ��echo�ŵĸߵ�ƽ
		}
		HAL_TIM_Base_Start(&TIM4_Handler);//������ʱ��4����ʼ����

		while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12)==1){//�ȴ�������ģ��echo�Ÿߵ�ƽ����
			if(TIM4->CNT>MAX_TIME*100){//��ʱ��ǰ�˳���*100�Ǹ��ݶ�ʱ����Ƶ�����
				break;
			}
		}
		count=TIM4->CNT;//�ȱ��涨ʱ���ļ���ֵ
		HAL_TIM_Base_Stop(&TIM4_Handler);//�رն�ʱ��4
		TIM4->CNT=0;//��ն�ʱ��4����

		if(count>=MAX_TIME*100){//�������������
			buffer[0]=MAX_TIME*100;
		}
		buffer[0]=count&0xFF;//�Ͱ�λ
		buffer[1]=pwm;

		HAL_UART_Transmit(&UART1_Handler,buffer,sizeof(buffer),1000);//���ڷ�������
		//delay_ms(500);

		//�����������led�ͷ�����
		if (count<=MAX_TIME*50){//����ڱ���������
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);//PB1��0�������
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET);//PC1��0������������
		}
		
		if(rotation){//�ж���ת�������޸�ռ�ձ�
			pwm+=1;
			TIM3->CCR1=pwm;//�޸�ռ�ձ�
			delay_ms(30);
		}else{
			pwm-=1;
			TIM3->CCR1=pwm;//�޸�ռ�ձ�
			delay_ms(30);
		}

		if(pwm==250){
			rotation=0;//pwm����250�󣬷���Ϊ��ת
		}			
		if(pwm==50){
			rotation=1;//pwm����50�󣬷����Ϊ��ת
		}

		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);//PB1��1�������
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET);//PC1��1���������ر�
	}
}
 

