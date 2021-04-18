import requests
session = requests.Session()

gate_UUID = "LOREMIPSUM"

sensor_data =   {
    "Gateway": gate_UUID,
    "Devices": 
    [
        {
            "Device":15533,
            "Sensors":
            [
                {
                    "Type": 1,
                    "DType": 1,
                    "data": "3.55"
                },
                {
                    "Type": 2,
                    "DType": 1,
                    "data": "5"
                }
            ]
        },
        {
            "Device":16874,
            "Sensors":
            [
                {
                    "Type": 1,
                    "DType": 1,
                    "data": "3.55"
                },
                {
                    "Type": 2,
                    "DType": 1,
                    "data": "5"
                },
                                {
                    "Type": 3,
                    "DType": 1,
                    "data": "3.55"
                },
                {
                    "Type": 4,
                    "DType": 1,
                    "data": "5"
                }
            ]
        },
        {
            "Device":14875,
            "Sensors":
            [
                {
                    "Type": 3,
                    "DType": 1,
                    "data": "3.55"
                },
                {
                    "Type": 4,
                    "DType": 1,
                    "data": "5"
                }
            ]
        }
    ]

}

get_data = {
    "Gateway":gate_UUID
}

r = session.post('http://localhost:8005/data', json=sensor_data)
r = session.get('http://localhost:8005/data', json=get_data)


print(r.status_code)

print(r.json())