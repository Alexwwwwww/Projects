from selenium import webdriver
from time import sleep
from selenium.webdriver.common.by import By
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.support import expected_conditions as EC
import time,datetime

USERNAME=" "
PASSWORD=" "
  
def login():
    # login my sis
    driver.get("https://sisprod.psft.ust.hk/psp/SISPROD/EMPLOYEE/HRMS/c/SA_LEARNER_SERVICES.SSS_STUDENT_CENTER.GBL?pslnkid=Z_HC_SSS_STUDENT_CENTER_LNK&FolderPath=PORTAL_ROOT_OBJECT.Z_HC_SSS_STUDENT_CENTER_LNK&IsFolder=false&IgnoreParamTempl=FolderPath%2cIsFolder")

    while True:
        try:
            if driver.find_element_by_xpath('//*[@id="userNameInput"]'):
                driver.find_element_by_xpath('//*[@id="userNameInput"]').send_keys(USERNAME)
                driver.find_element_by_xpath('//*[@id="passwordInput"]').send_keys(PASSWORD)
                driver.find_element_by_xpath('//*[@id="submitButton"]').click()
                break
        except:
            print("2FA.waiting for the page to load...")
    # can find shopping cart only by switch into frame
    while True:
        try:
            # if main page of student center has been loaded
            if driver.find_element_by_xpath('//*[@id="ptifrmtgtframe"]'):
                # enter the frame for academic
                driver.switch_to.frame(driver.find_element_by_xpath('//*[@id="ptifrmtgtframe"]'))
                print("LOGIN success")
                break
        except:
            print("loading")
            time.sleep(0.2)
    
    
def enroll(times):
##  go to the shopping cart
    while True:
        try:
            if driver.find_element(By.XPATH,'//*[@id="DERIVED_SSS_SCL_SSS_ENRL_CART$276$"]'):
                driver.find_element(By.XPATH,'//*[@id="DERIVED_SSS_SCL_SSS_ENRL_CART$276$"]').click()
                break
        except:
            print("Could not find the shopping cart")

##    TO BE CHANGED to other sems

    while True:
        try:
            if driver.find_element(By.XPATH,'//*[@id="SSR_DUMMY_RECV1$sels$1$$0"]'):
                driver.find_element(By.XPATH,'//*[@id="SSR_DUMMY_RECV1$sels$1$$0"]').click()
                break
        except:
            print("Could not find the semester")
    #   CONTINUE
    while True:
        try:
            if driver.find_element(By.XPATH,'//*[@id="DERIVED_SSS_SCT_SSR_PB_GO"]'):
                driver.find_element(By.XPATH,'//*[@id="DERIVED_SSS_SCT_SSR_PB_GO"]').click()
                break
        except:
            print("Could not press continue")
 
    # SELECT ALL
    while True:
        try:
            if driver.find_element_by_xpath('//*[@id="P_SELECT$0"]'):
                for i in range(10):
                    checkbox_id= '//*[@id="P_SELECT$'+ str(i) +'"]'
                    try:
                        if driver.find_element(By.XPATH,checkbox_id):
                            driver.find_element(By.XPATH,checkbox_id).click()
                    except:
                        print("skipping...")
                # clicking enroll`
                driver.find_element(By.XPATH,'//*[@id="DERIVED_REGFRM1_LINK_ADD_ENRL"]').click()
                break
        except:
            print("loading the page")

    now = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')
    while True:
        try:
            if now>times:
                driver.find_element(By.XPATH,'//*[@id="DERIVED_REGFRM1_SSR_PB_SUBMIT"]').click()
                break
        except:
            print("enrollment has not started")
            sleep(0.1)

    
    

if __name__=='__main__':
    driver=webdriver.Chrome()   # using browser chrome
    times = input("please enter the starting time of enrolment in format 2021-12-27 23:21:59.022000\n")
    USERNAME = input("please enter your ITSC\n")
    PASSWORD = input("please enter your password\n")
    login()
    enroll(times)
    print("enrolment successful")

