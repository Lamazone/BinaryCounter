#include <QHBoxLayout>
#include "counterwidget.h"
#include "config.h"
#include "gpio.h"
#include <QGridLayout>

CounterWidget::CounterWidget(QWidget *parent)
    : QWidget(parent), m_count(0), m_timer(this)
{
    setWindowTitle("BinaryCounter");
    resize(300, 200);
    QHBoxLayout* layout = new QHBoxLayout(this);
    setLayout(layout);

    m_display = new QLCDNumber(this);

    // QLCDNumber zur Anzeige des Zaehlerstandes dezimal:

    m_gpio = new Gpio(this);


    connect(&m_timer, &QTimer::timeout, this, &CounterWidget::updateCounter);
    m_timer.start(T_UPDATE);
}

CounterWidget::~CounterWidget()
{

}

void CounterWidget::updateCounter()
{
    int n = 0;

        for (auto pin : BUTTONS)
        {
            int state = !m_gpio->get(pin);
            if(m_gpio->edgeDetect(state, true, n))
            {
                if(n==0)
                {
                    m_counter--;
                    if(m_counter == -1)
                    {
                        m_counter = 15;
                    }
                    m_gpio->set(m_counter);
                }
                else if(n==1)
                {
                    m_counter=0;
                    m_gpio->set(m_counter);
                }
                else if(n==2)
                {
                    m_counter++;
                    if(m_counter == 16)
                    {
                        m_counter = 0;
                    }
                    m_gpio->set(m_counter);
                }
            }

            m_display->display(m_counter);
        }
}


