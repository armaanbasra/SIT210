from flask import Flask
from dash import Dash, dcc, html
from dash.dependencies import Input, Output
import plotly.graph_objs as go
import threading
import serial
import time
import re
from datetime import datetime
import requests

API_TOKEN = "8186713604:AAHCBiHIxyeBxlOzc2NJT_9hdJYpRC-Gfws"
CHAT_ID = "1700949297"

def send_telegram_message(message):

    try:
        url = f"https://api.telegram.org/bot{API_TOKEN}/sendMessage"
        data = {
            "chat_id": CHAT_ID,
            "text": message,
            "parse_mode": "HTML"
        }
        response = requests.post(url, data=data, timeout=10)
        return response.status_code == 200
    except Exception as e:
        print(f"❌ Telegram error: {e}")
        return False

server = Flask(__name__)
app = Dash(__name__, server=server)

try:
    arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    time.sleep(2)
    print("✅ Arduino Connected!")
    send_telegram_message("🚀 <b>Water Monitoring System Started!</b>\n📊 Now monitoring water quality...")
except:
    print("❌ Arduino not connected")
    arduino = None

tds_values = []
temp_values = []
hardness_values = []
timestamps = []
pump_status = "OFF"
notification_sent = False

def read_serial():
    global pump_status, notification_sent

    while True:
        if arduino and arduino.in_waiting > 0:
            line = arduino.readline().decode('utf-8', errors='ignore').strip()
            if line:
                match = re.search(r'Temp:\s*([\d.]+).*TDS:\s*(\d+)', line)
                if match:
                    temp = float(match.group(1))
                    tds = int(match.group(2))
                    hardness = tds * 0.7

                    temp_values.append(temp)
                    tds_values.append(tds)
                    hardness_values.append(hardness)
                    timestamps.append(datetime.now().strftime("%H:%M:%S"))

                    old_status = pump_status
                    if tds > 300:
                        pump_status = "ON"

                        if old_status != "ON" and not notification_sent:
                            message = f"🚨 <b>PUMP ACTIVATED!</b>\n\n💧 TDS Level: <b>{tds} ppm</b>\n🌡️ Temperature: <b>{temp:.1f}°C</b>\n⚖️ Hardness: <b>{hardness:.0f} mg/L</b>\n⏰ Time: {datetime.now().strftime('%H:%M:%S')}"
                            if send_telegram_message(message):
                                print("✅ Pump activation notification sent!")
                                notification_sent = True
                    else:
                        pump_status = "OFF"
                        notification_sent = False

                    # Keep only latest 30 readings
                    if len(tds_values) > 30:
                        tds_values.pop(0)
                        temp_values.pop(0)
                        hardness_values.pop(0)
                        timestamps.pop(0)
                    print(f"📊 {datetime.now().strftime('%H:%M:%S')} - Temp: {temp}°C, TDS: {tds}ppm, Hardness: {hardness:.0f}mg/L, Pump: {pump_status}")
        time.sleep(1)

threading.Thread(target=read_serial, daemon=True).start()


app.layout = html.Div([

    html.Div([
        html.H1("💧 Water Quality Monitor", style={
            'textAlign': 'center',
            'color': '#2c3e50',
            'marginBottom': '10px',
            'fontSize': '2.5rem',
            'fontWeight': '700'
        })
    ], style={
        'background': 'linear-gradient(135deg, #667eea 0%, #764ba2 100%)',
        'padding': '30px',
        'borderRadius': '15px 15px 0 0',
        'color': 'white'
    }),

    html.Div([

        html.Div([
            html.Div([
                html.H3("🌡️ Temperature", style={'color': '#2c3e50', 'marginBottom': '10px'}),
                html.Div(id='temp-reading', style={
                    'fontSize': '2.5rem',
                    'fontWeight': 'bold',
                    'color': '#e74c3c',
                    'margin': '10px 0'
                })
            ], style={
                'padding': '25px',
                'background': 'white',
                'borderRadius': '12px',
                'boxShadow': '0 4px 15px rgba(0,0,0,0.1)',
                'textAlign': 'center',
                'borderLeft': '5px solid #e74c3c',
                'height': '120px',
                'display': 'flex',
                'flexDirection': 'column',
                'justifyContent': 'center'
            }),
        ], style={'width': '24%', 'display': 'inline-block', 'padding': '10px'}),


        html.Div([
            html.Div([
                html.H3("💧 TDS Level", style={'color': '#2c3e50', 'marginBottom': '10px'}),
                html.Div(id='tds-reading', style={
                    'fontSize': '2.5rem',
                    'fontWeight': 'bold',
                    'color': '#3498db',
                    'margin': '10px 0'
                })
            ], style={
                'padding': '25px',
                'background': 'white',
                'borderRadius': '12px',
                'boxShadow': '0 4px 15px rgba(0,0,0,0.1)',
                'textAlign': 'center',
                'borderLeft': '5px solid #3498db',
                'height': '120px',
                'display': 'flex',
                'flexDirection': 'column',
                'justifyContent': 'center'
            }),
        ], style={'width': '24%', 'display': 'inline-block', 'padding': '10px'}),


        html.Div([
            html.Div([
                html.H3("⚖️ Hardness", style={'color': '#2c3e50', 'marginBottom': '10px'}),
                html.Div(id='hardness-reading', style={
                    'fontSize': '2.5rem',
                    'fontWeight': 'bold',
                    'color': '#9b59b6',
                    'margin': '10px 0'
                })
            ], style={
                'padding': '25px',
                'background': 'white',
                'borderRadius': '12px',
                'boxShadow': '0 4px 15px rgba(0,0,0,0.1)',
                'textAlign': 'center',
                'borderLeft': '5px solid #9b59b6',
                'height': '120px',
                'display': 'flex',
                'flexDirection': 'column',
                'justifyContent': 'center'
            }),
        ], style={'width': '24%', 'display': 'inline-block', 'padding': '10px'}),

        html.Div([
            html.Div([
                html.H3("🔧 Pump Status", style={'color': '#2c3e50', 'marginBottom': '10px'}),
                html.Div(id='pump-status', style={
                    'fontSize': '2.5rem',
                    'fontWeight': 'bold',
                    'margin': '10px 0'
                }),
                html.Div("📱 Phone alerts active", style={
                    'color': '#27ae60',
                    'fontSize': '12px',
                    'marginTop': '5px',
                    'fontWeight': 'bold'
                })
            ], style={
                'padding': '25px',
                'background': 'white',
                'borderRadius': '12px',
                'boxShadow': '0 4px 15px rgba(0,0,0,0.1)',
                'textAlign': 'center',
                'borderLeft': '5px solid #27ae60',
                'height': '120px',
                'display': 'flex',
                'flexDirection': 'column',
                'justifyContent': 'center'
            }),
        ], style={'width': '24%', 'display': 'inline-block', 'padding': '10px'}),
    ], style={
        'textAlign': 'center',
        'margin': '20px auto',
        'maxWidth': '1200px'
    }),


    html.Div([
        html.Div([
            html.Div([
                html.H3("TDS Trend", style={'color': '#2c3e50', 'marginBottom': '15px'}),
                dcc.Graph(id='tds-graph', style={'height': '300px'})
            ], style={
                'padding': '20px',
                'background': 'white',
                'borderRadius': '12px',
                'boxShadow': '0 4px 15px rgba(0,0,0,0.1)',
                'marginBottom': '20px'
            }),
        ], style={'width': '48%', 'display': 'inline-block', 'padding': '10px'}),

        html.Div([
            html.Div([
                html.H3("Temperature Trend", style={'color': '#2c3e50', 'marginBottom': '15px'}),
                dcc.Graph(id='temp-graph', style={'height': '300px'})
            ], style={
                'padding': '20px',
                'background': 'white',
                'borderRadius': '12px',
                'boxShadow': '0 4px 15px rgba(0,0,0,0.1)',
                'marginBottom': '20px'
            }),
        ], style={'width': '48%', 'display': 'inline-block', 'padding': '10px'}),
    ], style={
        'textAlign': 'center',
        'margin': '20px auto',
        'maxWidth': '1200px'
    }),


    html.Div([
        html.P([
            "Last Updated: ",
            html.Span(id="update-time", style={'fontWeight': 'bold'}),
            " | Auto-refresh: 10 seconds | 📱 Phone Alerts: ACTIVE"
        ], style={
            'textAlign': 'center',
            'color': '#7f8c8d',
            'padding': '15px',
            'background': '#f8f9fa',
            'borderRadius': '8px',
            'margin': '20px'
        })
    ]),

    dcc.Interval(id='update-interval', interval=10000, n_intervals=0)
], style={
    'backgroundColor': '#ecf0f1',
    'minHeight': '100vh',
    'fontFamily': 'Arial, sans-serif',
    'margin': '0',
    'padding': '0'
})


@app.callback(
    [Output('tds-graph', 'figure'),
     Output('temp-graph', 'figure'),
     Output('temp-reading', 'children'),
     Output('tds-reading', 'children'),
     Output('hardness-reading', 'children'),
     Output('pump-status', 'children'),
     Output('update-time', 'children')],
    [Input('update-interval', 'n_intervals')]
)
def update_dashboard(n):
    global pump_status

    if len(tds_values) == 0:
        empty_fig = go.Figure()
        empty_fig.update_layout(
            plot_bgcolor='rgba(0,0,0,0)',
            paper_bgcolor='rgba(0,0,0,0)',
            xaxis=dict(showgrid=False, zeroline=False, showticklabels=False),
            yaxis=dict(showgrid=False, zeroline=False, showticklabels=False),
            annotations=[dict(
                text="Waiting for sensor data...",
                xref="paper", yref="paper",
                x=0.5, y=0.5, xanchor='center', yanchor='middle',
                showarrow=False,
                font=dict(size=16, color="#7f8c8d")
            )]
        )
        return empty_fig, empty_fig, "--°C", "--ppm", "--mg/L", "🟢 OFF", "No data"


    tds_fig = go.Figure(data=[
        go.Scatter(
            x=timestamps,
            y=tds_values,
            mode='lines+markers',
            name='TDS',
            line=dict(color='#3498db', width=3),
            marker=dict(size=6, color='#2980b9')
        )
    ])
    tds_fig.update_layout(
        plot_bgcolor='rgba(0,0,0,0)',
        paper_bgcolor='rgba(0,0,0,0)',
        font=dict(color='#2c3e50'),
        margin=dict(l=40, r=40, t=30, b=40),
        xaxis=dict(title='Time', gridcolor='#e1e1e1'),
        yaxis=dict(title='ppm', gridcolor='#e1e1e1'),
        hovermode='x unified'
    )

    temp_fig = go.Figure(data=[
        go.Scatter(
            x=timestamps,
            y=temp_values,
            mode='lines+markers',
            name='Temperature',
            line=dict(color='#e74c3c', width=3),
            marker=dict(size=6, color='#c0392b')
        )
    ])
    temp_fig.update_layout(
        plot_bgcolor='rgba(0,0,0,0)',
        paper_bgcolor='rgba(0,0,0,0)',
        font=dict(color='#2c3e50'),
        margin=dict(l=40, r=40, t=30, b=40),
        xaxis=dict(title='Time', gridcolor='#e1e1e1'),
        yaxis=dict(title='°C', gridcolor='#e1e1e1'),
        hovermode='x unified'
    )

    current_temp = f"{temp_values[-1]:.1f}°C"
    current_tds = f"{tds_values[-1]}ppm"
    current_hardness = f"{hardness_values[-1]:.0f}mg/L"

    if pump_status == "ON":
        pump_display = "🔴 ON"
    else:
        pump_display = "🟢 OFF"

    return (
        tds_fig,
        temp_fig,
        current_temp,
        current_tds,
        current_hardness,
        pump_display,
        datetime.now().strftime("%H:%M:%S")
    )

if __name__ == "__main__":
    print("🚀 Starting Dashboard with Phone Notifications...")
    print("📊 Open: http://192.168.1.98:8050")
    print("📱 Phone alerts will be sent when pump activates")
    print("🔧 Pump activates at 300+ ppm")
    app.run(host='0.0.0.0', port=8050, debug=False)
