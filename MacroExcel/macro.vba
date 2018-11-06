Dim Row
Dim flagConnect As Boolean
Dim TimeStart
Dim TimeLast
Dim TimeAdd
Dim cc
Dim offsetCC
Dim WsConfig As Worksheet
    

Private Sub Absorcao_Click()
    Call clearSheet
    Call AskForLogs
    WsConfig.Range("I2") = "Absorção (miliAbs)"
End Sub

Private Sub btn1_Click()
    Stamp.SendData (WsConfig.Range("B2"))
End Sub
Private Sub btn2_Click()
    Stamp.SendData (WsConfig.Range("B3"))
End Sub
Private Sub btn3_Click()
    Stamp.SendData (WsConfig.Range("B4"))
End Sub
Private Sub btn4_Click()
    Stamp.SendData (WsConfig.Range("B5"))
End Sub
Private Sub btn5_Click()
    Stamp.SendData (WsConfig.Range("B6") + "#" + param1.Value + "#" + param2.Value + "#" + param3.Value + "#" + param4.Value)
    txtStatus2 = ("Enviado: " + param1.Value + "#" + param2.Value + "#" + param3.Value + "#" + param4.Value)
End Sub
Private Sub cboBAUD_Change()
    Stamp.Disconnect
    cmdConnect.Caption = "Connectar"
    flagConnect = False
    txtStatus2 = "Desconectado"
End Sub
Private Sub cboPort_Change()
   Stamp.Disconnect
   cmdConnect.Caption = "Conectar"
   flagConnect = False
   txtStatus2 = "Desconectado"
End Sub
Private Sub cmdClear_Click()
    Beep
    Call clearSheet
   
End Sub
Private Sub cmdConnect_Click()
    Stamp.Register = "Parallax:StampDAQ:A1F31"
    Stamp.Visible = False
    Stamp.Visible = True
    If flagConnect = False Then
      On Error GoTo ConnectErr
      Stamp.Baud = cboBAUD
      Stamp.Port = cboPort
      Stamp.DTREnabled = CBool(chkDTR)
      Stamp.Connect
      Stamp.Disconnect
      Stamp.Connect
      cmdConnect.Caption = "Desconectar"
      flagConnect = True
      txtStatus2 = "Conectado"
      offsetCC = 0
      Call SaveSetting("plx-daq", "app", "port", cboPort.Text)
      Call SaveSetting("plx-daq", "app", "baud", cboBAUD.Text)
      listLogs.Clear
      listSelectedLogs.Clear
      Stamp.SendData ("H")
    Else
      Stamp.Disconnect
      cmdConnect.Caption = "Conectar"
      flagConnect = False
      txtStatus2 = "Desconectado"
    End If
Exit Sub

ConnectErr:
Call MsgBox("Nao foi possivel conectar." & vbCrLf & "Verifique a porta e Baudrate", vbExclamation)
End Sub

Private Sub chkDump_Click()
    Beep
End Sub

Private Sub cmdResetTimer_Click()
    TimeStart = Timer
    TimeLast = Timer
    TimeAdd = 0
End Sub

Private Sub AskForLogs()
    Dim logsConcat As String
    Dim tipo As String
    
    If Absorcao.Value = True Then
        tipo = "A"
    End If
    
    If Transmissao.Value = True Then
        tipo = "T"
    End If
    
    If Real.Value = True Then
        tipo = "R"
    End If
    
    logsConcat = "L," + tipo
    For iCnt = 0 To listSelectedLogs.ListCount - 1
        logsConcat = logsConcat + "," + listSelectedLogs.List(iCnt)
    Next
    Stamp.SendData (logsConcat + ",\n")
End Sub

Private Sub CommandButton2_Click()
    listSelectedLogs.Clear
    listLogs.Clear
    Stamp.SendData ("H")
    Call clearSheet
End Sub


Private Sub Real_Click()
    Call clearSheet
    Call AskForLogs
    WsConfig.Range("I2") = "Intensidade"
End Sub

Private Sub Remove_Click()
    'Variable declaration
    Dim iCnt As Integer
    
    'Get Selcted Items from ListBox1 to ListBox2
    For iCnt = 0 To listSelectedLogs.ListCount - 1
        'Check ListBox Item has selcted or not
        If listSelectedLogs.Selected(iCnt) = True Then
            listLogs.AddItem listSelectedLogs.List(iCnt)
            listSelectedLogs.RemoveItem (iCnt)
            Call SortListByItemData
        End If
    Next
    Call clearSheet
    Call AskForLogs
End Sub

Private Sub Seleciona_Click()
 'Variable declaration
    Dim iCnt As Integer
    
    'Get Selcted Items from ListBox1 to ListBox2
    For iCnt = 0 To listLogs.ListCount - 1
        'Check ListBox Item has selcted or not
        If listLogs.Selected(iCnt) = True Then
            listSelectedLogs.AddItem listLogs.List(iCnt)
            listLogs.RemoveItem (iCnt)
            Call SortListByItemData
        End If
    Next
    Call clearSheet
    Call AskForLogs
End Sub

Private Sub SortListByItemData()
    Dim i As Long
    Dim j As Long
    Dim temp As Variant
    Dim SA As Variant
    Dim SB As Variant
    Dim DataValA() As String
    Dim DataValB() As String
    Dim DataA() As String
    Dim DataB() As String
    Dim IA As Integer
    Dim IB As Integer
    
    
    With Me.listLogs
        For j = 0 To listLogs.ListCount - 2
            For i = 0 To listLogs.ListCount - 2
                SA = listLogs.List(i)
                SB = listLogs.List(i + 1)
                DataA = Split(SA, ".")
                DataB = Split(SB, ".")
                DataValA = Split(DataA(0), "g")
                DataValB = Split(DataB(0), "g")
                IA = Int(DataValA(1))
                IB = Int(DataValB(1))
                If IA > IB Then
                    temp = .List(i)
                    .List(i) = .List(i + 1)
                    .List(i + 1) = temp
                End If
            Next i
        Next j
    End With
    With Me.listSelectedLogs
        For j = 0 To listSelectedLogs.ListCount - 2
            For i = 0 To listLogs.ListCount - 2
                SA = listLogs.List(i)
                SB = listLogs.List(i + 1)
                DataA = Split(SA, ".")
                DataB = Split(SB, ".")
                DataValA = Split(DataA(0), "g")
                DataValB = Split(DataB(0), "g")
                IA = Int(DataValA(1))
                IB = Int(DataValB(1))
                If IA > IB Then
                    temp = .List(i)
                    .List(i) = .List(i + 1)
                    .List(i + 1) = temp
                End If
            Next i
        Next j
    End With
End Sub

Private Sub stamp_CommError()
' On comm error beep and display
    MsgBox ("Erro de comunicacao")
    txtStatus2 = "Erro na formatacao dos dados ou na porta"
End Sub

Private Sub stamp_DataError()
    txtStatus2 = "Erro ao processar comando verifique a sintaxe"
End Sub

Private Sub stamp_DataReady()
    
    On Error GoTo Data_Error
    Dim DataVal() As String
    Dim Data As String
    
    While Stamp.gotData = True
      Data = Stamp.GetData
      If Data <> "" Then
        DataVal = Split(Data, ",")
        Select Case DataVal(0)
            Case "CLEARDATA"
                Beep
                txtStatus2 = "Clearing sheet"
                Call clearSheet
                Row = 1
            Case "RESETTIMER"
                TimeStart = Timer
                TimeLast = Timer
                TimeAdd = 0
                txtStatus2 = "Timer Reset"
            Case "LABEL"
                cc = countChar(Data, ",")
                txtStatus2 = "Setting labels"
                For X = 1 To cc Mod 27
                  Worksheets(1).Range(Chr(64 + X + offsetCC) & CStr(1)).Value = ReplaceData(DataVal(X))
                Next
                Row = 1
            Case "DATA"
                cc = countChar(Data, ",")
                Row = Row + 1
                txtStatus2 = "Accepting data for Row " & (Row - 1)
                If Row < 65000 Then
                    For X = 1 To cc Mod 27
                      Worksheets(1).Range(Chr(64 + X + offsetCC) & CStr(Row)).Value = ReplaceData(DataVal(X))
                    Next
                End If
            Case "RESET"
                txtStatus2 = "Reset - Limpando Dados!"
                chkReset.Value = False
            Case "RESETROW"
                Row = 1
            Case "MSG"
                txtStatus1 = DataVal(1)
            Case "CELL"
                Select Case DataVal(1)
                    Case "GET"
                          Stamp.SendData (Worksheets(1).Range(DataVal(2)).Value)
                          txtStatus2 = "Getting Cell " & DataVal(2)
                    Case "SET"
                          Worksheets(1).Range(DataVal(2)).Value = ReplaceData(DataVal(3))
                          txtStatus2 = "Setting Cell " & DataVal(3)
                End Select
            Case "OFFSET_COLUMN"
                offsetCC = offsetCC + DataVal(1)
            Case "RESET_COLUMN"
                offsetCC = 0
            Case "LOG"
                listLogs.AddItem DataVal(1)
            End Select
        End If
    Wend
    Exit Sub
Data_Error:
End Sub

Private Sub clearSheet()

  Dim sht As Worksheet
  Set sht = Worksheets(1)
  col_cnt = sht.UsedRange.Columns.Count
  If col_cnt = 0 Then
    col_cnt = 1
  End If

  sht.Range(sht.Cells(1, 1), sht.Cells(sht.UsedRange.Rows.Count, col_cnt)).Clear

End Sub

Private Function countChar(stringIn As String, stringChar As String)
    For X = 1 To Len(stringIn)
        If Mid(stringIn, X, 1) = stringChar Then
            countChar = countChar + 1
        End If
    Next
End Function

Private Function ReplaceData(strData)
    If Timer < TimeLast Then
        If TimeAdd = 0 Then
            TimeAdd = (86400# - TimeStart)
        Else
            TimeAdd = TimeAdd + 86400#
        End If
        TimeStart = 0
    End If
    
    TimeLast = Timer
    strData = Replace(strData, "TIMER", Str(Timer - TimeStart + TimeAdd))
    strData = Replace(strData, "TIME", Time)
    strData = Replace(strData, "DATE", Date)
    ReplaceData = strData
End Function
Function WorksheetExists(sName As String) As Boolean
    WorksheetExists = Evaluate("ISREF('" & sName & "'!A1)")
End Function

Private Sub initConfig()
    ''Verificando se a planilha de configuracao existe
    If WorksheetExists("Config") Then
        Set WsConfig = ActiveWorkbook.Sheets("Config")
    Else
        Set WsConfig = ActiveWorkbook.Sheets.Add(After:=Worksheets(Worksheets.Count))
        Call criaWsConfig
    End If
    
End Sub

Private Sub criaWsConfig()
    WsConfig.Name = "Config"
    WsConfig.Range("A1") = "Label"
    WsConfig.Range("B1") = "Val Arduino"
    WsConfig.Range("C1") = "Descricao"
    
    WsConfig.Range("A2") = "Rotina A"
    WsConfig.Range("B2") = "A"
    WsConfig.Range("C2") = "Envia o valor A para o arduino"
    
    WsConfig.Range("A3") = "Rotina B"
    WsConfig.Range("B3") = "B"
    WsConfig.Range("C3") = "Envia o valor B para o arduino"
    
    WsConfig.Range("A4") = "Rotina C"
    WsConfig.Range("B4") = "C"
    WsConfig.Range("C4") = "Envia o valor C para o arduino"
    
    WsConfig.Range("A5") = "Rotina D"
    WsConfig.Range("B5") = "D"
    WsConfig.Range("C5") = "Envia o valor D para o arduino"
    
    WsConfig.Range("A6") = "Envia Parametros"
    WsConfig.Range("B6") = "E"
    WsConfig.Range("C6") = "Envia o valor E para o arduino junto com os dois parametros concatenados"
    
    WsConfig.Range("A7") = "Parametro 1"
    WsConfig.Range("B7") = "-"
    WsConfig.Range("C7") = "Descrição do primeiro parametro"
    
    WsConfig.Range("A8") = "Parametro 2"
    WsConfig.Range("B8") = "-"
    WsConfig.Range("C8") = "Descrição do segundo parametro"
    
    WsConfig.Range("A9") = "Parametro 3"
    WsConfig.Range("B9") = "-"
    WsConfig.Range("C9") = "Descrição do primeiro parametro"
    
    WsConfig.Range("A10") = "Parametro 4"
    WsConfig.Range("B10") = "-"
    WsConfig.Range("C10") = "Descrição do primeiro parametro"
    
    WsConfig.Range("A12") = "Para que os valores mudem reinicie o PLX-DAQ"
    WsConfig.Range("A13") = "Ctrl+q para iniciar o PLX-DAQ"
End Sub


Private Sub Transmissao_Click()
    Call clearSheet
    Call AskForLogs
    WsConfig.Range("I2") = "Transmissão (%)"
End Sub

Private Sub userform_Initialize()
    
    Call initConfig
    
    cboPort.Text = GetSetting("plx-daq", "app", "port", "1")
    cboBAUD.AddItem ("300")
    cboBAUD.AddItem ("600")
    cboBAUD.AddItem ("1200")
    cboBAUD.AddItem ("2400")
    cboBAUD.AddItem ("4800")
    cboBAUD.AddItem ("9600")
    cboBAUD.AddItem ("14400")
    cboBAUD.AddItem ("19200")
    cboBAUD.AddItem ("38400")
    cboBAUD.AddItem ("56000")
    
    cboBAUD.Text = GetSetting("plx-daq", "app", "baud", 9600)
    
    btn1.Caption = WsConfig.Range("A2")
    btn2.Caption = WsConfig.Range("A3")
    btn3.Caption = WsConfig.Range("A4")
    btn4.Caption = WsConfig.Range("A5")
    btn5.Caption = WsConfig.Range("A6")
    
    labelParam1.Caption = WsConfig.Range("A7")
    labelParam2.Caption = WsConfig.Range("A8")
    labelParam3.Caption = WsConfig.Range("A9")
    labelParam4.Caption = WsConfig.Range("A10")
    
    Stamp.Register = "Parallax:StampDAQ:A1F31"
    Stamp.Visible = False
    Stamp.Visible = True
    Row = 1
    cc = 10
End Sub
