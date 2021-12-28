from selenium import webdriver
import datetime
import time

directory_of_driver = input("please enter the path of your browser driver: ")
username = input("please enter your username: ")
password = input("please enter your password: ")

print("please enter the enrollment time")
hour = input("hour: ")
minute = input("minute: ")
designated_time_str = (datetime.datetime.now().strftime("%Y-%m-%d")) + ' ' + hour + ':' + minute + ':' + '00.000000'
designated_time = int(hour) * 3600 + int(minute) * 60

def login():
    browser.get("https://sisprod.psft.ust.hk/psp/SISPROD/EMPLOYEE/HRMS/c/SA_LEARNER_SERVICES.SSS_STUDENT_CENTER.GBL?pslnkid=Z_HC_SSS_STUDENT_CENTER_LNK&FolderPath=PORTAL_ROOT_OBJECT.Z_HC_SSS_STUDENT_CENTER_LNK&IsFolder=false&IgnoreParamTempl=FolderPath,IsFolder")
    # user name
    while True:
        # polling, 0.2 seconds per cycle
        try:
            # if user name input box has been found
            if browser.find_element_by_xpath('//*[@id="userNameInput"]'):
                print("load success")
                # click input box for user name
                browser.find_element_by_xpath('//*[@id="userNameInput"]').click()
                # input user name
                browser.find_element_by_xpath('//*[@id="userNameInput"]').send_keys(username)
                # click input box for password
                browser.find_element_by_xpath('//*[@id="passwordInput"]').click()
                # input for password
                browser.find_element_by_xpath('//*[@id="passwordInput"]').send_keys(password)
                # click submit button
                browser.find_element_by_xpath('//*[@id="submitButton"]').click()
                break
        except:
            print("waiting for the page to load...")
            time.sleep(0.2)
    while True:
        # polling, 0.5 seconds per cycle
        try:
            # if main page of student center has been loaded
            if browser.find_element_by_xpath('//*[@id="ptifrmtgtframe"]'):
                # enter the frame for academic
                browser.switch_to.frame(browser.find_element_by_xpath('//*[@id="ptifrmtgtframe"]'))
                print("load success")
                break
        except:
            print("waiting for the page to load...")
            time.sleep(0.5)
    
    return True


# Enroll at designated time
def enroll(designated_time_str):
    # if button for enrollment shopping chart has been found
    if browser.find_element_by_xpath('//*[@id="DERIVED_SSS_SCL_SSS_ENRL_CART$276$"]'):
        # click the button for enrollment shopping chart
        browser.find_element_by_xpath('//*[@id="DERIVED_SSS_SCL_SSS_ENRL_CART$276$"]').click()

    while True:
        # polling, 0.2 seconds per cycle
        try:
            # if button for term selection has been found
            if browser.find_element_by_xpath('//*[@id="SSR_DUMMY_RECV1$sels$1$$0"]'):
                # select winter term
                browser.find_element_by_xpath('//*[@id="SSR_DUMMY_RECV1$sels$1$$0"]').click()
                # click "continue" button
                browser.find_element_by_xpath('//*[@id="DERIVED_SSS_SCT_SSR_PB_GO"]').click()
                break
        except:
            print("waiting for the page to load...")
            time.sleep(0.2)

    while True:
        # polling, 0.2 seconds per cycle
        try:
            # if course selection button has been loaded
            if browser.find_element_by_xpath('//*[@id="P_SELECT$0"]'):
                # select all the courses in the chart
                for i in range(20):
                    course_selector_id = '//*[@id="P_SELECT$'+ str(i) +'"]'
                    try:
                        if browser.find_element_by_xpath(course_selector_id):
                            browser.find_element_by_xpath(course_selector_id).click()
                except:
                        # skip if the number is not found
                        print("skipping...")
                # click "enroll" button
                browser.find_element_by_xpath('//*[@id="DERIVED_REGFRM1_LINK_ADD_ENRL"]').click()
                break
        except:
            print("waiting for the page to load...")
            time.sleep(0.2)

    while True:
        # polling, 0.2 seconds per cycle
        try:
            # if "finish enroll" button has been loaded
            if browser.find_element_by_xpath('//*[@id="DERIVED_REGFRM1_SSR_PB_SUBMIT"]'):
                break
        except:
            print("waiting for the page to load...")
            time.sleep(0.2)

    # start waiting for the enrollment time
    while True:
        # polling, no waiting time

        # get current time, in the form of yy-mm-dd hh:mm:ss.ffffff
        now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
        if now > designated_time_str: # string comparison
            browser.find_element_by_xpath('//*[@id="DERIVED_REGFRM1_SSR_PB_SUBMIT"]').click()
            break
        else:
            # print the time left
            print("seconds left: ", end="")
            now_time = datetime.datetime.now().strftime("%H:%M:%S.%f")
            now_time = now_time.split(':')
            now_time = int(now_time[0])*3600 + int(now_time[1])*60 + float(now_time[2])
            print(designated_time - now_time)

# main function

# initialize the browser
browser = webdriver.Chrome(executable_path = directory_of_driver)

login_success = login()

if not login_success:
    print("fail to login")
    exit(-1)

print("login success")
enroll(designated_time_str)
