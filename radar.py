import serial,pygame,sys,math

#初始化pygame
pygame.init()
screen = pygame.display.set_mode((1200,700))
pygame.display.set_caption("超声波雷达 作者：幽蓝伊梦")
#pygame：0----->x
#        |
#        ↓
#        y

#定义一个队列来收集串口接收到的数据
list_line=[]
for i in range(40):
    list_line.append((100,50))#填充无用的数据

#定义两个列表用于存储连续变化的颜色来实现颜色渐变，例如暗绿色到绿色
green_color=[]
red_color=[]
for i in range(39,-1,-1):#每次画多少根线，range参数就是多少
    color=255-i*6#颜色由最黑到最鲜艳
    green_color.append((0,color,0))
    red_color.append((color,0,0))

serial_port="COM4" #COM口
serial_baudrate=76800  #波特率
ser=serial.Serial(serial_port,serial_baudrate,timeout=0.5) #打开串口，超时时间为0.5秒


while True:
    for event in pygame.event.get():  #监听事件
        if event.type == pygame.QUIT: #如果关闭窗口则退出程序
            pygame.quit()
            sys.exit()
    #读取串口数据
    data=ser.readline()#一行有两个数据，超声波发送接收时间，占空比，单位和数值都根据单片机程序来修改
    distance=data[0]*34/100/2#data[0]是TIM4的计数值，100=1ms，并根据超声波波速换算距离cm
    #print(distance)
    rad=math.radians(data[1]*0.9-45)#data[1]是占空比值，50->250(对应角度：0°->180°)，计算成角度后转换为弧度

    list_line.append((distance,rad))#数据添加到队列中
    if len(list_line)>40:#一次性最多绘制40根线
        list_line.pop(0)
 
    #背景覆盖
    screen.fill((0,0,0))

    for i in range(40):
        if list_line[i][0]<20:#距离小于20，红色线
            pygame.draw.line(screen,red_color[i],(600,620),(600-600*math.cos(list_line[i][1]),620-600*math.sin(list_line[i][1])),3) 
        else:
            pygame.draw.line(screen,green_color[i],(600,620),(600-600*math.cos(list_line[i][1]),620-600*math.sin(list_line[i][1])),3)

    if distance<20:#对应的距离和警告文本
        screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",25).render("已发现！",1,"red"),(180,630))
        screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",25).render(str(distance)+"cm",1,"green"),(490,630))
    else:
        screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",25).render("未发现！",1,"green"),(180,630))
        screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",25).render("xx",1,"green"),(490,630))

    #画底部横线和文字信息
    pygame.draw.line(screen,"green",(0,620),(1200,620),5) 
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",25).render("目 标：",1,"green"),(100,630))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",25).render("距 离：",1,"green"),(410,630))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",25).render("角 度：",1,"green"),(680,630))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",25).render(str(int(data[1]*0.9-45))+"°",1,"green"),(760,630))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",25).render("幽蓝伊梦",1,(92,255,255)),(930,630))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",18).render("求三连~",1,(251,114,153)),(1050,630))
    #画半圆和特殊角直线
    pygame.draw.arc(screen,"green",(600-125,620-125,250,250),math.radians(0),math.radians(181), width=3)
    pygame.draw.arc(screen,"green",(600-250,620-250,500,500),math.radians(0),math.radians(181), width=3)
    pygame.draw.arc(screen,"green",(600-375,620-375,750,750),math.radians(0),math.radians(180.5), width=3)
    pygame.draw.arc(screen,"green",(600-500,620-500,1000,1000),math.radians(0),math.radians(180.5), width=3)
    pygame.draw.line(screen,"green",(600,620),(600-600*math.cos(math.radians(30)),620-600*math.sin(math.radians(30))),3) 
    pygame.draw.line(screen,"green",(600,620),(600-600*math.cos(math.radians(60)),620-600*math.sin(math.radians(60))),3) 
    pygame.draw.line(screen,"green",(600,620),(600,20),3) 
    pygame.draw.line(screen,"green",(600,620),(600-600*math.cos(math.radians(120)),620-600*math.sin(math.radians(120))),3) 
    pygame.draw.line(screen,"green",(600,620),(600-600*math.cos(math.radians(150)),620-600*math.sin(math.radians(150))),3) 
    #距离和角度文字
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",30).render("30°",1,"green"),(45,320))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",30).render("60°",1,"green"),(240,90))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",30).render("90°",1,"green"),(535,10))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",30).render("120°",1,"green"),(820,90))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",30).render("150°",1,"green"),(1120,320))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",20).render("5cm",1,"green"),(730,590))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",20).render("10cm",1,"green"),(855,590))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",20).render("15cm",1,"green"),(980,590))
    screen.blit(pygame.font.Font("SourceHanSansSC-Regular.otf",20).render("20cm",1,"green"),(1105,590))
    #更新屏幕
    pygame.display.update()
    #运行速度，配合单片机的速度调整
    pygame.time.Clock().tick(5000)
