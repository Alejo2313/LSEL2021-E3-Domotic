from control_flow import Controll_flow
import pytest

@pytest.fixture
def initial_fsm():
    fsm = Controll_flow("GW")
    return fsm

def test_initial_state(initial_fsm):
    assert initial_fsm.state == "CONFIG"

def test_to_idle(initial_fsm):
    initial_fsm.start()
    assert initial_fsm.state == "IDLE"

def test_new_data(initial_fsm):
    initial_fsm.to_IDLE()
    initial_fsm.newData()
    assert initial_fsm.state == "PROCESS_DATA"

def test_needResponse(initial_fsm):
    initial_fsm.to_PROCESS_DATA()
    initial_fsm.needResponse()
    assert initial_fsm.state == "PROCESS_DATA"

def test_no_need_response(initial_fsm):
    initial_fsm.to_PROCESS_DATA()
    initial_fsm.noNeedResponse()
    assert initial_fsm.state == "IDLE"

def test_resp_ready(initial_fsm):
    initial_fsm.to_PROCESS_DATA()
    initial_fsm.respReady()
    assert initial_fsm.state == "IDLE"

def test_new_order(initial_fsm):
    initial_fsm.to_IDLE()
    initial_fsm.newOrder()
    assert initial_fsm.state == "PROCESS_ORDER"

def test_needData(initial_fsm):
    initial_fsm.to_PROCESS_ORDER()
    initial_fsm.needData()
    assert initial_fsm.state == "PROCESS_ORDER"

def test_no_need_data(initial_fsm):
    initial_fsm.to_PROCESS_ORDER()
    initial_fsm.noNeedData()
    assert initial_fsm.state == "IDLE"

def test_data_ready(initial_fsm):
    initial_fsm.to_PROCESS_ORDER()
    initial_fsm.dataReady()
    assert initial_fsm.state == "IDLE"